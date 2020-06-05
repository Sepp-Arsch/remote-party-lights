package webserver.handler;

import org.json.JSONArray;
import org.json.JSONObject;
import webserver.RunWebServer;
import webserver.data.LightSettings;
import webserver.data.SerialSettings;

import java.awt.*;

public class JsonHandler{
    private RunWebServer server;

    public void setServer(RunWebServer server) {
        this.server = server;
    }

    public enum CMD {
        CONNECT,
        DISCONNECT,
        PORTS,
        GETLIGHT,
        SETLIGHT,
        GETSERIAL,
        SETSERIAL,
        STATUS
    }

    public String handle(String jsonString) {
        try {
            JSONObject jsonIn = new JSONObject(jsonString);
            CMD cmd = CMD.valueOf(jsonIn.getString("CMD"));

            JSONObject jsonOut = new JSONObject();
            switch (cmd) {
                case CONNECT: connect(jsonIn, jsonOut); break;
                case DISCONNECT: disconnect(jsonOut); break;
                case STATUS: status(jsonOut); break;
                case PORTS: ports(jsonOut); break;
                case GETLIGHT: getLight(jsonOut); break;
                case SETLIGHT: setLight(jsonIn, jsonOut); break;
                case GETSERIAL: getSerial(jsonOut); break;
                case SETSERIAL: setSerial(jsonIn, jsonOut); break;
            }

            return jsonOut.toString();
        } catch (Exception e) {
            return "{\"error\":\"could not parse received data\", \"stacktrace\":\"" +
                    e.getMessage() + "\"}";
        }
    }

    private void connect(JSONObject in, JSONObject out) {
        out.append(CMD.CONNECT.toString(), server.connectSerialPort(in.getString("PORT")));
    }

    private void disconnect(JSONObject out) {
        out.append(CMD.CONNECT.toString(), server.disconnectSerialPort());
    }

    private void ports(JSONObject out) {
        JSONArray array = new JSONArray();
        array.put(server.getAvailableSerialPorts());
        out.append(CMD.PORTS.toString(), array);
    }

    private void getLight(JSONObject out) {
        LightSettings lightSettings = server.getLightSettings();
        JSONObject obj = new JSONObject();
        obj.append("MODE", lightSettings.mode.toString());
        obj.append("R", lightSettings.color.getRed());
        obj.append("G", lightSettings.color.getGreen());
        obj.append("B", lightSettings.color.getGreen());
        obj.append("A", lightSettings.color.getAlpha());
        obj.append("INTERVAL", lightSettings.interval);
        out.append(CMD.GETLIGHT.toString(), obj);
    }

    private void setLight(JSONObject in, JSONObject out) {
        LightSettings lightSettings = server.getLightSettings();
        lightSettings.color = new Color(in.getInt("R"), in.getInt("G"), in.getInt("B"),
                in.getInt("A"));
        lightSettings.mode = LightSettings.MODE.valueOf(in.getString("MODE"));
        lightSettings.interval = in.getInt("INTERVAL");

        out.append(CMD.SETLIGHT.toString(), true);
    }

    private void getSerial(JSONObject out) {
        SerialSettings serialSettings = server.getSerialSettings();
        JSONObject obj = new JSONObject();
        obj.append("DELAY", serialSettings.delay);
        obj.append("BAUDRATE", serialSettings.baudRate);
        obj.append("DATABITS", serialSettings.dataBits);
        obj.append("STOPBITS", serialSettings.stopBits);
        obj.append("PARITY", serialSettings.parity);
        out.append(CMD.GETSERIAL.toString(), obj);
    }

    private void setSerial(JSONObject in, JSONObject out) {
        SerialSettings serialSettings = server.getSerialSettings();
        serialSettings.delay = in.getInt("DELAY");
        serialSettings.baudRate = in.getInt("BAUDRATE");
        serialSettings.dataBits = in.getInt("DATABITS");
        serialSettings.stopBits = in.getInt("STOPBITS");
        serialSettings.parity = in.getInt("PARITY");

        out.append(CMD.SETSERIAL.toString(), true);
    }

    private void status(JSONObject out) {
        String serialStatus = server.getSerialStatus();
        out.append("PORT", serialStatus);
        out.append("CONNECTED", !serialStatus.equals("DISCONNECTED"));
    }
}
