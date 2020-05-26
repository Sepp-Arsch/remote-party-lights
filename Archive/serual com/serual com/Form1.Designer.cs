namespace serual_com
{
    partial class Form1
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.Send = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.on = new System.Windows.Forms.Button();
            this.Rot = new System.Windows.Forms.TrackBar();
            this.An = new System.Windows.Forms.Button();
            this.Gruen = new System.Windows.Forms.TrackBar();
            this.Blau = new System.Windows.Forms.TrackBar();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.Grün = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.Helligkeit = new System.Windows.Forms.TrackBar();
            this.Blink = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.Interval = new System.Windows.Forms.TrackBar();
            this.RealTime = new System.Windows.Forms.CheckBox();
            this.off = new System.Windows.Forms.Button();
            this.Send.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Rot)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Gruen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Blau)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.Grün.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Helligkeit)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Interval)).BeginInit();
            this.SuspendLayout();
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(78, 21);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 24);
            this.comboBox1.TabIndex = 0;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(231, 21);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Open";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(231, 56);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Close";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // comboBox2
            // 
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Items.AddRange(new object[] {
            "9600",
            "115200"});
            this.comboBox2.Location = new System.Drawing.Point(78, 56);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(121, 24);
            this.comboBox2.TabIndex = 3;
            // 
            // Send
            // 
            this.Send.Controls.Add(this.button3);
            this.Send.Controls.Add(this.textBox1);
            this.Send.Location = new System.Drawing.Point(28, 123);
            this.Send.Name = "Send";
            this.Send.Size = new System.Drawing.Size(200, 235);
            this.Send.TabIndex = 4;
            this.Send.TabStop = false;
            this.Send.Text = "groupBox1";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(46, 195);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 1;
            this.button3.Text = "Send";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(6, 32);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(175, 132);
            this.textBox1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(38, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 17);
            this.label1.TabIndex = 5;
            this.label1.Text = "Port";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 59);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(66, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "Baudrate";
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(205, 19);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(20, 61);
            this.progressBar1.TabIndex = 7;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.comboBox1);
            this.groupBox2.Controls.Add(this.progressBar1);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.button2);
            this.groupBox2.Controls.Add(this.comboBox2);
            this.groupBox2.Controls.Add(this.button1);
            this.groupBox2.Location = new System.Drawing.Point(29, 26);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(321, 91);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Connection";
            // 
            // on
            // 
            this.on.Location = new System.Drawing.Point(404, 47);
            this.on.Name = "on";
            this.on.Size = new System.Drawing.Size(75, 23);
            this.on.TabIndex = 9;
            this.on.Text = "All ON";
            this.on.UseVisualStyleBackColor = true;
            this.on.Click += new System.EventHandler(this.button4_Click);
            // 
            // Rot
            // 
            this.Rot.Location = new System.Drawing.Point(6, 19);
            this.Rot.Maximum = 99;
            this.Rot.Name = "Rot";
            this.Rot.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.Rot.Size = new System.Drawing.Size(56, 212);
            this.Rot.TabIndex = 10;
            this.Rot.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.Rot.Scroll += new System.EventHandler(this.Rot_Scroll);
            // 
            // An
            // 
            this.An.Location = new System.Drawing.Point(879, 42);
            this.An.Name = "An";
            this.An.Size = new System.Drawing.Size(75, 23);
            this.An.TabIndex = 11;
            this.An.Text = "An";
            this.An.UseVisualStyleBackColor = true;
            this.An.Click += new System.EventHandler(this.button5_Click);
            // 
            // Gruen
            // 
            this.Gruen.Location = new System.Drawing.Point(6, 19);
            this.Gruen.Maximum = 99;
            this.Gruen.Name = "Gruen";
            this.Gruen.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.Gruen.Size = new System.Drawing.Size(56, 212);
            this.Gruen.TabIndex = 12;
            this.Gruen.TickStyle = System.Windows.Forms.TickStyle.Both;
            // 
            // Blau
            // 
            this.Blau.Location = new System.Drawing.Point(6, 21);
            this.Blau.Maximum = 99;
            this.Blau.Name = "Blau";
            this.Blau.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.Blau.Size = new System.Drawing.Size(56, 212);
            this.Blau.TabIndex = 13;
            this.Blau.TickStyle = System.Windows.Forms.TickStyle.Both;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.Rot);
            this.groupBox1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox1.Location = new System.Drawing.Point(499, 71);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(63, 239);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Rot";
            // 
            // Grün
            // 
            this.Grün.Controls.Add(this.Gruen);
            this.Grün.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Grün.Location = new System.Drawing.Point(568, 71);
            this.Grün.Name = "Grün";
            this.Grün.Size = new System.Drawing.Size(63, 239);
            this.Grün.TabIndex = 15;
            this.Grün.TabStop = false;
            this.Grün.Text = "Grün";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.Blau);
            this.groupBox3.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox3.Location = new System.Drawing.Point(637, 71);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(63, 239);
            this.groupBox3.TabIndex = 16;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Blau";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.Helligkeit);
            this.groupBox4.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox4.Location = new System.Drawing.Point(706, 71);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(63, 239);
            this.groupBox4.TabIndex = 17;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Helligkeit";
            // 
            // Helligkeit
            // 
            this.Helligkeit.Location = new System.Drawing.Point(6, 21);
            this.Helligkeit.Maximum = 99;
            this.Helligkeit.Name = "Helligkeit";
            this.Helligkeit.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.Helligkeit.Size = new System.Drawing.Size(56, 212);
            this.Helligkeit.TabIndex = 13;
            this.Helligkeit.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.Helligkeit.Value = 99;
            // 
            // Blink
            // 
            this.Blink.Location = new System.Drawing.Point(879, 71);
            this.Blink.Name = "Blink";
            this.Blink.Size = new System.Drawing.Size(75, 23);
            this.Blink.TabIndex = 18;
            this.Blink.Text = "Blink";
            this.Blink.UseVisualStyleBackColor = true;
            this.Blink.Click += new System.EventHandler(this.button6_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.Interval);
            this.groupBox5.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox5.Location = new System.Drawing.Point(774, 71);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(63, 239);
            this.groupBox5.TabIndex = 18;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Interval";
            // 
            // Interval
            // 
            this.Interval.Location = new System.Drawing.Point(6, 21);
            this.Interval.Maximum = 99;
            this.Interval.Name = "Interval";
            this.Interval.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.Interval.Size = new System.Drawing.Size(56, 212);
            this.Interval.TabIndex = 13;
            this.Interval.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.Interval.Value = 99;
            // 
            // RealTime
            // 
            this.RealTime.AutoSize = true;
            this.RealTime.Location = new System.Drawing.Point(313, 184);
            this.RealTime.Name = "RealTime";
            this.RealTime.Size = new System.Drawing.Size(90, 21);
            this.RealTime.TabIndex = 19;
            this.RealTime.Text = "RealTime";
            this.RealTime.UseVisualStyleBackColor = true;
            this.RealTime.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // off
            // 
            this.off.Location = new System.Drawing.Point(404, 76);
            this.off.Name = "off";
            this.off.Size = new System.Drawing.Size(75, 23);
            this.off.TabIndex = 20;
            this.off.Text = "All OFF";
            this.off.UseVisualStyleBackColor = true;
            this.off.Click += new System.EventHandler(this.off_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1187, 591);
            this.Controls.Add(this.off);
            this.Controls.Add(this.RealTime);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.Blink);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.Grün);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.An);
            this.Controls.Add(this.on);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.Send);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Send.ResumeLayout(false);
            this.Send.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Rot)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Gruen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Blau)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.Grün.ResumeLayout(false);
            this.Grün.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Helligkeit)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Interval)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.GroupBox Send;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button on;
        private System.Windows.Forms.TrackBar Rot;
        private System.Windows.Forms.Button An;
        private System.Windows.Forms.TrackBar Gruen;
        private System.Windows.Forms.TrackBar Blau;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox Grün;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TrackBar Helligkeit;
        private System.Windows.Forms.Button Blink;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TrackBar Interval;
        private System.Windows.Forms.CheckBox RealTime;
        private System.Windows.Forms.Button off;
    }
}

