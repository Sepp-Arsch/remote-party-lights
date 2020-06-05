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

    public int encode() {
        int fourthParameter = mode == MODE.CONTINUOUS || mode == MODE.COLOR ?
                map(color.getTransparency()) : interval;
        return (int) (mode.value * Math.pow(10, 8)
                + map(color.getRed()) * Math.pow(10, 6)
                + map(color.getGreen()) * Math.pow(10, 4)
                + map(color.getBlue()) * Math.pow(10, 2)
                + fourthParameter);
    }

    private int map(int x) {
        return x * 99 / 255;
    }
}
