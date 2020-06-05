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

    public Color color = new Color(255, 255, 255, 255);
    public MODE mode = MODE.CONTINUOUS;
    public int interval = 0; // must be between 0-99
}
