package webserver.handler;

import webserver.RunWebServer;

public class JsonHandler{
    private RunWebServer server;

    public void setServer(RunWebServer server) {
        this.server = server;
    }

    public enum CMD {
        SERIALACTION,
        SERIALPORTS,
        LIGHT
    }

    public String handle(String json) {
        //server.testA();
        return "{}";
    }
}
