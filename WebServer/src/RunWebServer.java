import handler.JsonHandler;
import handler.SerialHandler;

import java.awt.*;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URI;
import java.util.Date;

public class RunWebServer implements Runnable {
    static final File WEB_ROOT = new File(".");
    static final String DEFAULT_FILE = "index.html";

    // port to listen connection
    static final int PORT = 80;

    // verbose mode
    static final boolean verbose = true;

    // Client Connection via Socket Class
    private Socket connect;

    // Controlling the following handlers
    private JsonHandler jsonHandler;
    private SerialHandler serialHandler;

    public RunWebServer(Socket c) {
        connect = c;
        jsonHandler = new JsonHandler();
        serialHandler = new SerialHandler();
    }

    public static void main(String[] args) {
        try {
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
            System.err.println();
            out = new PrintWriter(connect.getOutputStream());
            dataOut = new BufferedOutputStream(connect.getOutputStream());

            byte[] buffer = new byte[6000]; // hard coded to maximum HTTP payload of 6kb
            in.read(buffer);
            String input = new String(buffer, "UTF-8");

            if (input.isEmpty())
                return;

            String[] inputLines = input.split(System.lineSeparator());
            String httpHead = inputLines[0];
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
                out.println(jsonHandler.handle(jsonData));
                out.flush();

                if (verbose)
                    System.out.println("Responded to POST request");
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
                connect.close(); // we close socket connection
            } catch (Exception e) {
                System.err.println("Error closing stream : " + e.getMessage());
            }

            if (verbose)
                System.out.println("Connection closed.\n");
        }
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
        else
            return "text/plain";
    }
}