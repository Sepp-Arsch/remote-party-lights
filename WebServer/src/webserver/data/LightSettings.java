package webserver.data;

import java.awt.*;

public class LightSettings {
    public final Color color;
    public final int alphaMin;
    public final MODE mode;
    public final int intervalMin;   // must be between 0-99
    public final int intervalMax;   // must be between 0-99
    public final int idFrom;        // must be between 0-99
    public final int idTo;          // must be between 0-99
    public final int idPattern;     // must be between 0-99

    public LightSettings() {
        this(new Color(255, 255, 255, 255),0, MODE.SET, 0, 0, 0, 99, 0);
    }

    public LightSettings(Color color, int alphaMin, MODE mode, int intervalMin, int intervalMax, int idFrom, int idTo,
                         int idPattern) {
        this.color = color;
        this.alphaMin = alphaMin;
        this.mode = mode;
        this.intervalMin = intervalMin;
        this.intervalMax = intervalMax;
        this.idFrom = idFrom;
        this.idTo = idTo;
        this.idPattern = idPattern;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null || obj.getClass() != this.getClass())
            return false;
        LightSettings lsObj = (LightSettings) obj;
        return (lsObj.mode == this.mode && lsObj.color.equals(this.color) &&
                lsObj.intervalMin == this.intervalMin && lsObj.intervalMax == this.intervalMax &&
                lsObj.alphaMin == this.alphaMin && lsObj.idFrom == this.idFrom && lsObj.idTo == this.idTo &&
                lsObj.idPattern == this.idPattern);
    }

    public enum MODE {
        KILL(98),
        SET(1),
        OFF(2),
        BLINK(3),
        BLINK_T(4),
        PULSE(5),
        PULSE_T(6),
        FLASH(7),
        FLASH_R(8),
        SET_R(9),
        PULSE_R(11);

        public final int value;

        private MODE(int value) {
            this.value = value;
        }
    }
}
