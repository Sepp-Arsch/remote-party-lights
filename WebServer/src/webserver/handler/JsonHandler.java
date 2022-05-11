package webserver.handler;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import webserver.RunWebServer;
import webserver.data.LightSettings;
import webserver.data.SerialSettings;

import java.awt.*;

public class JsonHandler {
    public static String cmdKey = "CMD";

    private RunWebServer server;

    public void setServer(RunWebServer server) {
        this.server = server;
    }

    public String handle(String jsonString) {
        try {
            JSONObject jsonIn = new JSONObject(jsonString);
            Object cmds = jsonIn.get(cmdKey);
            JSONArray cmdsArray;
            if (cmds instanceof JSONArray)
                cmdsArray = jsonIn.getJSONArray(cmdKey);
            else if (cmds instanceof String)
                cmdsArray = new JSONArray().put(jsonIn.getString(cmdKey));
            else
                throw new JSONException("Unknown command, query POST {\"CMD\":\"LISTOPTIONS\" for all commands.");

            JSONObject jsonOut = new JSONObject();
            for (Object cmd : cmdsArray) {
                switch (CMD.valueOf(cmd.toString())) {
                    case CONNECT:
                        connect(jsonIn, jsonOut);
                        break;
                    case DISCONNECT:
                        disconnect(jsonOut);
                        break;
                    case STATUS:
                        status(jsonOut);
                        break;
                    case PORTS:
                        ports(jsonOut);
                        break;
                    case GETLIGHT:
                        getLight(jsonOut);
                        break;
                    case SETLIGHT:
                        setLight(jsonIn, jsonOut);
                        break;
                    case TRANSMIT:
                        transmit(jsonIn);
                        break;
                    case GETSERIAL:
                        getSerial(jsonOut);
                        break;
                    case SETSERIAL:
                        setSerial(jsonIn, jsonOut);
                        break;
                    case LISTOPTIONS:
                        listOptions(jsonOut);
                }
            }
            return jsonOut.toString();
        } catch (EnumConstantNotPresentException | JSONException e) {
            return "{\"error\":" + e.getLocalizedMessage() + ", \"input\":\"" + jsonString + "\"}";
        }
    }

    private void connect(JSONObject in, JSONObject out) {
        out.put(CMD.CONNECT.toString(), server.connectSerialPort(in.getString("PORT")));
    }

    private void disconnect(JSONObject out) {
        out.put(CMD.DISCONNECT.toString(), server.disconnectSerialPort());
    }

    private void ports(JSONObject out) {
        out.put(CMD.PORTS.toString(), server.getAvailableSerialPorts());
    }

    private void getLight(JSONObject out) {
        LightSettings lightSettings = server.getLightSettings();
        JSONObject obj = new JSONObject();
        obj.put("MODE", lightSettings.mode.toString());
        obj.put("R", lightSettings.color.getRed());
        obj.put("G", lightSettings.color.getGreen());
        obj.put("B", lightSettings.color.getBlue());
        obj.put("A", lightSettings.color.getAlpha());
        obj.put("Amin", lightSettings.alphaMin);
        obj.put("Imin", lightSettings.intervalMin);
        obj.put("Imax", lightSettings.intervalMax);
        obj.put("IDfrom", lightSettings.idFrom);
        obj.put("IDto", lightSettings.idTo);
        obj.put("IDpattern", lightSettings.idPattern);
        out.put(CMD.GETLIGHT.toString(), obj);
    }

    private void setLight(JSONObject in, JSONObject out) {
        server.setLightSettings(new LightSettings(
                new Color(in.getInt("R"), in.getInt("G"), in.getInt("B"), in.getInt("A")),
                in.getInt("Amin"),
                LightSettings.MODE.valueOf(in.getString("MODE")),
                in.getInt("Imin"),
                in.getInt("Imax"),
                in.getInt("IDfrom"),
                in.getInt("IDto"),
                in.getInt("IDpattern")
        ));
        out.put(CMD.SETLIGHT.toString(), true);
    }

    private void transmit(JSONObject in) {
        server.transmit(in.getString("MODE"));
    }

    private void getSerial(JSONObject out) {
        SerialSettings serialSettings = server.getSerialSettings();
        JSONObject obj = new JSONObject();
        obj.put("DELAY", serialSettings.delay);
        obj.put("BAUDRATE", serialSettings.baudRate);
        obj.put("DATABITS", serialSettings.dataBits);
        obj.put("STOPBITS", serialSettings.stopBits);
        obj.put("PARITY", serialSettings.parity);
        obj.put("ONLYSENDONCHANGE", serialSettings.onlySendOnChanges);
        out.put(CMD.GETSERIAL.toString(), obj);
    }

    private void setSerial(JSONObject in, JSONObject out) {
        server.setSerialSettings(new SerialSettings(
                in.getInt("DELAY"),
                in.getInt("BAUDRATE"),
                in.getInt("DATABITS"),
                in.getInt("STOPBITS"),
                in.getInt("PARITY"),
                in.getBoolean("ONLYSENDONCHANGE")
        ));
        out.put(CMD.SETSERIAL.toString(), true);
    }

    private void status(JSONObject out) {
        String serialStatus = server.getSerialStatus();
        JSONObject obj = new JSONObject();
        obj.put("PORT", serialStatus);
        obj.put("CONNECTED", !serialStatus.equals("DISCONNECTED"));
        out.put(CMD.STATUS.toString(), obj);
    }

    private void listOptions(JSONObject out) {
        JSONObject obj = new JSONObject();

        JSONArray arr = new JSONArray();
        for (CMD c : CMD.values())
            arr.put(c.toString());
        obj.put("CMD", arr);

        JSONArray arrMode = new JSONArray();
        for (LightSettings.MODE m : LightSettings.MODE.values())
            arrMode.put(m.toString());
        obj.put("MODE", arrMode);

        out.put(CMD.LISTOPTIONS.toString(), obj);
    }

    public enum CMD {
        CONNECT,
        DISCONNECT,
        PORTS,
        GETLIGHT,
        SETLIGHT,
        TRANSMIT,
        GETSERIAL,
        SETSERIAL,
        STATUS,
        LISTOPTIONS
    }
}
