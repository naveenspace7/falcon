namespace FalconDeploy
{
    partial class form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.copyBox = new System.Windows.Forms.CheckBox();
            this.buildBox = new System.Windows.Forms.CheckBox();
            this.deployButton = new System.Windows.Forms.Button();
            this.statusLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(42, 34);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(290, 46);
            this.label1.TabIndex = 0;
            this.label1.Text = "Falcon Installer";
            // 
            // copyBox
            // 
            this.copyBox.AutoSize = true;
            this.copyBox.Location = new System.Drawing.Point(65, 124);
            this.copyBox.Name = "copyBox";
            this.copyBox.Size = new System.Drawing.Size(71, 24);
            this.copyBox.TabIndex = 1;
            this.copyBox.Text = "Copy";
            this.copyBox.UseVisualStyleBackColor = true;
            // 
            // buildBox
            // 
            this.buildBox.AutoSize = true;
            this.buildBox.Location = new System.Drawing.Point(65, 169);
            this.buildBox.Name = "buildBox";
            this.buildBox.Size = new System.Drawing.Size(70, 24);
            this.buildBox.TabIndex = 2;
            this.buildBox.Text = "Build";
            this.buildBox.UseVisualStyleBackColor = true;
            // 
            // deployButton
            // 
            this.deployButton.Location = new System.Drawing.Point(50, 242);
            this.deployButton.Name = "deployButton";
            this.deployButton.Size = new System.Drawing.Size(282, 51);
            this.deployButton.TabIndex = 3;
            this.deployButton.Text = "DEPLOY";
            this.deployButton.UseVisualStyleBackColor = true;
            this.deployButton.Click += new System.EventHandler(this.deployButton_Click);
            // 
            // statusLabel
            // 
            this.statusLabel.AutoSize = true;
            this.statusLabel.Location = new System.Drawing.Point(18, 324);
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(0, 20);
            this.statusLabel.TabIndex = 4;
            // 
            // form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(370, 359);
            this.Controls.Add(this.statusLabel);
            this.Controls.Add(this.deployButton);
            this.Controls.Add(this.buildBox);
            this.Controls.Add(this.copyBox);
            this.Controls.Add(this.label1);
            this.Name = "form1";
            this.Text = "Deploy";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox copyBox;
        private System.Windows.Forms.CheckBox buildBox;
        private System.Windows.Forms.Button deployButton;
        private System.Windows.Forms.Label statusLabel;
    }
}

