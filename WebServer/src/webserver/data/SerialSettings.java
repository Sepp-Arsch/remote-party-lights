package webserver.data;

public class SerialSettings {
    public final int delay; // ms delay in Java Web Server -> Arduino Sender
    public final int baudRate;
    public final int dataBits;
    public final int stopBits;
    public final int parity;
    public final boolean onlySendOnChanges;

    public SerialSettings() {
            this(200, 9600, 22, 1, 0, true);
    }

    public SerialSettings(int delay, int baudRate, int dataBits, int stopBits, int parity, boolean onlySendOnChanges) {
        this.delay = delay;
        this.baudRate = baudRate;
        this.dataBits = dataBits;
        this.stopBits = stopBits;
        this.parity = parity;
        this.onlySendOnChanges = onlySendOnChanges;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null || obj.getClass() != this.getClass())
            return false;
        SerialSettings ssObj = (SerialSettings) obj;
        return (ssObj.delay == this.delay && ssObj.baudRate == this.baudRate && ssObj.dataBits == this.dataBits
                && ssObj.stopBits == this.stopBits && ssObj.parity == this.parity
                && ssObj.onlySendOnChanges == this.onlySendOnChanges);
    }
}
