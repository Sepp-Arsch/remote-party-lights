package webserver.handler;

import org.json.JSONObject;
import webserver.RunWebServer;
import webserver.data.LightSettings;
import webserver.data.SerialSettings;

import java.awt.*;

public class JsonHandler {
    private RunWebServer server;

    public void setServer(RunWebServer server) {
        this.server = server;
    }

    public String handle(String jsonString) {
        try {
            JSONObject jsonIn = new JSONObject(jsonString);
            CMD cmd = CMD.valueOf(jsonIn.getString("CMD"));

            JSONObject jsonOut = new JSONObject();
            switch (cmd) {
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
                case GETSERIAL:
                    getSerial(jsonOut);
                    break;
                case SETSERIAL:
                    setSerial(jsonIn, jsonOut);
                    break;
            }

            return jsonOut.toString();
        } catch (EnumConstantNotPresentException e) {
            return "{\"error\":\"could not parse received data\", \"stacktrace\":\"" +
                    e.getMessage() + "\"}";
        }
    }

    private void connect(JSONObject in, JSONObject out) {
        out.put(CMD.CONNECT.toString(), server.connectSerialPort(in.getString("PORT")));
    }

    private void disconnect(JSONObject out) {
        out.put(CMD.CONNECT.toString(), server.disconnectSerialPort());
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
        obj.put("B", lightSettings.color.getGreen());
        obj.put("A", lightSettings.color.getAlpha());
        obj.put("INTERVAL", lightSettings.interval);
        out.put(CMD.GETLIGHT.toString(), obj);
    }

    private void setLight(JSONObject in, JSONObject out) {
        server.setLightSettings(new LightSettings(
                new Color(in.getInt("R"), in.getInt("G"), in.getInt("B"), in.getInt("A")),
                LightSettings.MODE.valueOf(in.getString("MODE")),
                in.getInt("INTERVAL")
        ));
        out.put(CMD.SETLIGHT.toString(), true);
    }

    private void getSerial(JSONObject out) {
        SerialSettings serialSettings = server.getSerialSettings();
        JSONObject obj = new JSONObject();
        obj.put("DELAY", serialSettings.delay);
        obj.put("BAUDRATE", serialSettings.baudRate);
        obj.put("DATABITS", serialSettings.dataBits);
        obj.put("STOPBITS", serialSettings.stopBits);
        obj.put("PARITY", serialSettings.parity);
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
        out.put("PORT", serialStatus);
        out.put("CONNECTED", !serialStatus.equals("DISCONNECTED"));
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
}
