package webserver;

import webserver.data.LightSettings;
import webserver.data.SerialSettings;
import webserver.handler.JsonHandler;
import webserver.handler.SerialHandler;

import java.awt.*;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URI;
import java.util.Date;
import java.util.List;

public class RunWebServer implements Runnable {
    static final File WEB_ROOT = new File(".");
    static final String DEFAULT_FILE = "index.html";

    static final int PORT = 80;
    static final SerialHandler serialHandler = SerialHandler.getInstance();
    public static boolean verbose;

    private Socket connect;
    private JsonHandler jsonHandler;

    public RunWebServer(Socket c) {
        connect = c;
        jsonHandler = new JsonHandler();
    }

    public static void main(String[] args) {
        verbose = args.length > 0;
        try {
            // launch serial handler (singleton)
            Thread serialThread = new Thread(serialHandler);
            serialThread.start();

            // launch web server
            ServerSocket serverConnect = new ServerSocket(PORT);
            System.out.println("Server started. Listening for connections on port : " + PORT + "...\n");

            // open browser on hosted page
            try {
                Desktop.getDesktop().browse(URI.create("http://localhost:" + PORT));
            } catch (IOException e) {
                e.printStackTrace();
            }

            // create dedicated thread to manage the client connection
            while (true) {
                RunWebServer myServer = new RunWebServer(serverConnect.accept());
                myServer.jsonHandler.setServer(myServer);

                if (verbose)
                    System.out.println("Connection opened. (" + new Date() + ")");

                Thread thread = new Thread(myServer);
                thread.start();
            }
        } catch (IOException e) {
            System.err.println("Server Connection error : " + e.getMessage());
        }
    }

    @Override
    public void run() {
        InputStream in = null;
        PrintWriter out = null;
        BufferedOutputStream dataOut = null;
        String fileRequested = null;

        try {
            in = connect.getInputStream();
            out = new PrintWriter(connect.getOutputStream());
            dataOut = new BufferedOutputStream(connect.getOutputStream());

            byte[] buffer = new byte[2000]; // hard coded to maximum HTTP payload of 6kb
            in.read(buffer);
            String input = new String(buffer, "UTF-8").trim();

            if (input.isEmpty()) {
                if (verbose)
                    System.out.println("EMPTY HEAD");
                return;
            }

            String[] inputLines = input.split(System.lineSeparator());
            String httpHead = inputLines[0];

            if (verbose)
                System.out.println("HEAD: " + httpHead);
            String method = httpHead.split(" ")[0].toUpperCase();
            fileRequested = httpHead.split(" ")[1].toLowerCase();

            // we support only GET and HEAD methods, we check
            if (!method.equals("GET") && !method.equals("POST")) {
                if (verbose)
                    System.out.println("501 Not Implemented : " + method + " method.");
            } else if (method.equals("GET")) {
                if (fileRequested.endsWith("/"))
                    fileRequested += DEFAULT_FILE;

                File file = new File(WEB_ROOT, fileRequested);
                int fileLength = (int) file.length();
                String content = getContentType(fileRequested);

                // GET method so we return content
                byte[] fileData = readFileData(file, fileLength);

                // send HTTP Headers
                out.println("HTTP/1.1 200 OK");
                out.println("Server: Java HTTP Party Lights Server");
                out.println("Date: " + new Date());
                out.println("Content-type: " + content);
                out.println("Content-length: " + fileLength);
                out.println(); // blank line between headers and content, very important!
                out.flush(); // flush character output stream buffer

                dataOut.write(fileData, 0, fileLength);
                dataOut.flush();

                if (verbose)
                    System.out.println("File " + fileRequested);
            } else if (method.equals("POST")) {
                String jsonData = inputLines[inputLines.length - 1];
                String response = jsonHandler.handle(jsonData);
                out.println(response);
                out.flush();

                if (verbose) {
                    System.out.println("Incoming POST request: " + jsonData);
                    System.out.println("Outgoing POST response: " + response);
                }
            }

        } catch (FileNotFoundException fnfe) {
            if (verbose)
                System.out.println("File " + fileRequested + " not found");
        } catch (IOException ioe) {
            System.err.println("Server error : " + ioe);
        } finally {
            try {
                in.close();
                out.close();
                dataOut.close();
                connect.close();
            } catch (Exception e) {
                System.err.println("Error closing stream : " + e.getMessage());
            }

            if (verbose)
                System.out.println("Connection closed.\n");
        }
    }

    public boolean connectSerialPort(String port) {
        return serialHandler.connect(port);
    }

    public boolean disconnectSerialPort() {
        return serialHandler.disconnect();
    }

    public String getSerialStatus() {
        return serialHandler.getStatus();
    }

    public List<String> getAvailableSerialPorts() {
        return serialHandler.getAvailablePorts();
    }

    public SerialSettings getSerialSettings() {
        return serialHandler.getSerialSettings();
    }

    public void setSerialSettings(SerialSettings serialSettings) {
        serialHandler.setSerialSettings(serialSettings);
    }

    public LightSettings getLightSettings() {
        return serialHandler.getLightSettings();
    }

    public void setLightSettings(LightSettings lightSettings) {
        serialHandler.setLightSettings(lightSettings);
    }

    private byte[] readFileData(File file, int fileLength) throws IOException {
        FileInputStream fileIn = null;
        byte[] fileData = new byte[fileLength];

        try {
            fileIn = new FileInputStream(file);
            fileIn.read(fileData);
        } finally {
            if (fileIn != null)
                fileIn.close();
        }

        return fileData;
    }

    // return supported MIME Types
    private String getContentType(String fileRequested) {
        if (fileRequested.endsWith(".htm") || fileRequested.endsWith(".html"))
            return "text/html";
        else if (fileRequested.endsWith(".css"))
            return "text/css";
        else if (fileRequested.endsWith(".svg"))
            return "image/svg+xml";
        else
            return "text/plain";
    }
}