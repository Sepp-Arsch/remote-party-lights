package webserver.data;

import java.awt.*;

public class LightSettings {
    public enum MODE {
        CONTINUOUS(1),
        OFF(2),
        BLINK(3),
        BLINK_T(4),
        PULSE(5),
        PULSE_T(6),
        FLASH(7),
        FLASH_R(8),
        COLOR_R(9),
        COLOR(10),
        PULSE_R(11);

        public final int value;

        private MODE(int value) {
            this.value = value;
        }
    }

    public final Color color;
    public final MODE mode;
    public final int interval; // must be between 0-99

    public LightSettings() {
        this(new Color(255, 255, 255, 255), MODE.CONTINUOUS, 0);
    }

    public LightSettings(Color color, MODE mode, int interval) {
        this.color = color;
        this.mode = mode;
        this.interval = interval;
    }

    @Override
    public boolean equals(Object obj) {
        if(this == obj)
            return true;
        if(obj == null || obj.getClass()!= this.getClass())
            return false;
        LightSettings lsObj = (LightSettings) obj;
        return (lsObj.mode == this.mode && lsObj.color == this.color && lsObj.interval == this.interval);
    }
}
