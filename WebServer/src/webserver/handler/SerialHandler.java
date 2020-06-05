package webserver.handler;

import com.fazecast.jSerialComm.SerialPort;

import java.io.IOException;

/**
 * Singleton
 */
public class SerialHandler {
    private String port;

    private static SerialHandler INSTANCE;

    private SerialHandler() {
    }

    public synchronized static SerialHandler getInstance() {
        if(INSTANCE == null)
            INSTANCE = new SerialHandler();
        return INSTANCE;
    }



    public static void main(String[] args) throws IOException, InterruptedException {
        SerialPort[] a = SerialPort.getCommPorts();
        for (SerialPort i : a){
            System.out.println(i.getSystemPortName());
        }

//        SerialPort sp = SerialPort.getCommPort("COM3");
//        sp.setComPortParameters(9600, 8, 1, 0); // default connection settings for Arduino
//        sp.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0); // block until bytes can be written
//
//        if (sp.openPort()) {
//            System.out.println("Port is open :)");
//        } else {
//            System.out.println("Failed to open port :(");
//            return;
//        }
//
//        for (Integer i = 0; i < 50; ++i) {
//            sp.getOutputStream().write(i.byteValue());
//            sp.getOutputStream().flush();
//            System.out.println("Sent number: " + i);
//            Thread.sleep(500);
//        }
//
//        if (sp.closePort()) {
//            System.out.println("Port is closed :)");
//        } else {
//            System.out.println("Failed to close port :(");
//            return;
//        }

    }
}
