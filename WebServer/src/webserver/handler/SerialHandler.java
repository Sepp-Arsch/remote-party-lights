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
    private LightSettings lightSettings;
    private SerialSettings serialSettings;
    private SerialPort serialPort;

    private static SerialHandler INSTANCE;

    private SerialHandler() {
        this.lightSettings = new LightSettings();
        this.serialSettings = new SerialSettings();
    }

    public synchronized static SerialHandler getInstance() {
        if(INSTANCE == null)
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

//    public synchronized void setSerialSettings(SerialSettings serialSettings) {
//        this.serialSettings = serialSettings;
//    }

    public synchronized SerialSettings getSerialSettings() {
        return this.serialSettings;
    }

    public synchronized String getStatus() {
        if(serialPort.isOpen())
            return serialPort.getSystemPortName();
        return "DISCONNECTED";
    }

    public synchronized boolean disconnect() {
        return serialPort.closePort();
    }

//    public synchronized void setLightSettings(LightSettings lightSettings) {
//        this.lightSettings = lightSettings;
//    }

    public synchronized LightSettings getLightSettings() {
        return this.lightSettings;
    }

    public synchronized List<String> getAvailablePorts() {
        SerialPort[] ports = SerialPort.getCommPorts();
        List<String> list = new ArrayList<>();
        for (SerialPort port : ports)
            list.add(port.getSystemPortName());
        return list;
    }


    private int encode(LightSettings lightSettings) {
        int fourthParameter = lightSettings.mode == LightSettings.MODE.CONTINUOUS
                || lightSettings.mode == LightSettings.MODE.COLOR ?
                map(lightSettings.color.getTransparency()) : lightSettings.interval;
        return (int) (lightSettings.mode.value * Math.pow(10, 8)
                + map(lightSettings.color.getRed()) * Math.pow(10, 6)
                + map(lightSettings.color.getGreen()) * Math.pow(10, 4)
                + map(lightSettings.color.getBlue()) * Math.pow(10, 2)
                + fourthParameter);
    }

    private int map(int x) {
        return x * 99 / 255;
    }

    @Override
    public void run() {
        while(true) {
            try {
                // Send current LightSettings to Arduino every *delay* ms
                if (serialPort != null && serialPort.isOpen() && lightSettings != null) {
                    serialPort.getOutputStream().write(encode(lightSettings));
                    serialPort.getOutputStream().flush();
                }

                Thread.sleep(serialSettings.delay);
            } catch (InterruptedException | IOException e) {
                e.printStackTrace();
            }
        }
    }
}
