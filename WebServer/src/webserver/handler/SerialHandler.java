package webserver.handler;

import com.fazecast.jSerialComm.SerialPort;
import webserver.data.LightSettings;
import webserver.data.SerialSettings;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Singleton
 */
public class SerialHandler implements Runnable {
    private static volatile SerialHandler INSTANCE;
    private LightSettings lightSettings;
    private SerialSettings serialSettings;
    private SerialPort serialPort = null;
    private boolean lightSettingsChanges = false;

    private SerialHandler() {
        this.lightSettings = new LightSettings();
        this.serialSettings = new SerialSettings();
    }

    public synchronized static SerialHandler getInstance() {
        if (INSTANCE == null)
            INSTANCE = new SerialHandler();
        return INSTANCE;
    }

    public synchronized boolean connect(String port) {
        serialPort = SerialPort.getCommPort(port);
        serialPort.setComPortParameters(serialSettings.baudRate, serialSettings.dataBits, serialSettings.stopBits,
                serialSettings.parity);
        serialPort.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0);
        return serialPort.openPort();
    }

    public synchronized SerialSettings getSerialSettings() {
        return this.serialSettings;
    }

    public synchronized void setSerialSettings(SerialSettings serialSettings) {
        if (!this.serialSettings.equals(serialSettings))
            this.serialSettings = serialSettings;
    }

    public synchronized String getStatus() {
        if (serialPort != null && serialPort.isOpen())
            return serialPort.getSystemPortName();
        return "DISCONNECTED";
    }

    public synchronized boolean disconnect() {
        if (serialPort == null)
            return false;
        return serialPort.closePort();
    }

    public synchronized LightSettings getLightSettings() {
        return this.lightSettings;
    }

    public synchronized void setLightSettings(LightSettings lightSettings) {
        if (!this.lightSettings.equals(lightSettings)) {
            this.lightSettings = lightSettings;
            this.lightSettingsChanges = true;
        }
    }

    public synchronized List<String> getAvailablePorts() {
        SerialPort[] ports = SerialPort.getCommPorts();
        List<String> list = new ArrayList<>();
        for (SerialPort port : ports)
            list.add(port.getSystemPortName());
        return list;
    }

    private String encode(LightSettings lightSettings) {
        return codeSegment(lightSettings.mode.value) +
        codeSegment(map(lightSettings.color.getRed())) +
        codeSegment(map(lightSettings.color.getGreen())) +
        codeSegment(map(lightSettings.color.getBlue())) +
        codeSegment(map(lightSettings.color.getAlpha())) +
        codeSegment(map(lightSettings.alphaMin)) +
        codeSegment(mapTimeMs(lightSettings.intervalMin)) +
        codeSegment(mapTimeMs(lightSettings.intervalMax)) +
        codeSegment(lightSettings.idFrom) +
        codeSegment(lightSettings.idTo) +
        codeSegment(lightSettings.idPattern);
    }

    private String codeSegment(int code) {
        return String.format("%02d", code);
    }

    private int map(int x) {
        return x * 99 / 255;
    }

    private int mapTimeMs(int x) {
        return x / 100;
    }

    @Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(serialSettings.delay);

                // Do not send LightSettings if sending on change is activated but no change present
                if (serialSettings.onlySendOnChanges && !this.lightSettingsChanges)
                    continue;

                // Send current LightSettings to Arduino every *delay* ms
                if (serialPort != null && serialPort.isOpen() && lightSettings != null) {
                    System.out.println("Sending " + encode(lightSettings)
                            + " via serial port " + serialPort.getSystemPortName());

                    this.lightSettingsChanges = false;
                    serialPort.getOutputStream().write(encode(lightSettings).getBytes());
                    serialPort.getOutputStream().flush();
                }
            } catch (InterruptedException | IOException e) {
                e.printStackTrace();
            }
        }
    }
}
