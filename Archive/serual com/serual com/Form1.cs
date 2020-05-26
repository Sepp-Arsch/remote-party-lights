using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;


namespace serual_com
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            getAvailablePorts();
            if (comboBox1.Items.Count > 0)
            {
                comboBox1.SelectedIndex = 0;
            }

            if (comboBox2.Items.Count > 0)
            {
                comboBox2.SelectedIndex = 0;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (comboBox1.Text=="")
                {
                    textBox1.Text = "Com Port auswaehlen";
                }
                else 
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
                    serialPort1.Open();
                    progressBar1.Value = 100;
                    button1.Enabled = false;
                }
            }
            catch (UnauthorizedAccessException)
            {
                textBox1.Text = "Fehler";
            }
        }
        void getAvailablePorts()
        {
            String[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            progressBar1.Value = 0;
            button1.Enabled = true;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            serialPort1.WriteLine(textBox1.Text);
            textBox1.Text = "";
        }

        private void button4_Click(object sender, EventArgs e)
        {
            serialPort1.WriteLine(Convert.ToString(199999999));
        }

        private void button5_Click(object sender, EventArgs e)
        {
            int Befehl = 1;
            SendSerial(Befehl, Rot.Value, Gruen.Value, Blau.Value, Helligkeit.Value);
        }

        private void SendSerial(int Befehl, int var_1, int var_2, int var_3, int var_4)
        {
            uint SendData = Convert.ToUInt32(Befehl * 100000000 + var_1 * 1000000 + var_2 * 10000 + var_3 * 100 + var_4);
            serialPort1.WriteLine(Convert.ToString(SendData));
        }

        private void button6_Click(object sender, EventArgs e)
        {
            int Befehl = 3;
            SendSerial(Befehl, Rot.Value, Gruen.Value, Blau.Value, Interval.Value);
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void Rot_Scroll(object sender, EventArgs e)
        {

        }

        private void off_Click(object sender, EventArgs e)
        {
            serialPort1.WriteLine(Convert.ToString(299999999));
        }
    }
}
