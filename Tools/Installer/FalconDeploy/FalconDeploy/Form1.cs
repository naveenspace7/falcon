using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using WinSCP;
using Renci.SshNet;
using System.Net.NetworkInformation;

namespace FalconDeploy
{
    public partial class form1 : Form
    {
        private string hostPath, targetPath;
        private List<Tuple<string, string>> toCopy = new List<Tuple<string, string>>();

        private string host = "192.168.0.143";
        private string user = "pi";
        private string password = "raspberry";

        public form1()
        {
            InitializeComponent();
        }

        private void deployButton_Click(object sender, EventArgs e)
        {
            process_XML();

            if(copyBox.Checked)
            {
                ssh_command("mv ~/Falcon ~/Falcon.old");
                ssh_command("mkdir ~/Falcon");

                // Do copy action here
                copySource();

                if (buildBox.Checked)
                {
                    // Do building here
                    statusLabel.Text = "Building Libraries and Engine...";
                    ssh_command("chmod +777 ~/Falcon/build/installer_build");
                    ssh_command("~/Falcon/build/installer_build");
                    statusLabel.Text = "Done Building Libraries and Engines";
                }
            }            
        }

        // Open the XML and take up only the enabled source leaving the rest        
        public void process_XML()
        {
            XmlReader reader = XmlReader.Create(@"E:\Falcon\Tools\Installer\lookup.xml");
            while (reader.Read())
            {
                // Set the parent directories here
                if (reader.Name == "parent")
                {
                    hostPath = reader["host"];
                    targetPath = reader["target"];
                }

                if (reader.Name == "file")
                {
                    string toStr = targetPath; //+ Convert.ToString(reader["name"]) +"/";
                    string fromStr = hostPath + Convert.ToString(reader["name"]);
                    toCopy.Add(Tuple.Create(fromStr, toStr));                
                }
            }
        }

        // Used to copy the required source to the SBC for deployment
        private void copySource()
        {
            statusLabel.Text = "Copying Falcon source to the SBC....";
            try
            {
                // Setup session options
                SessionOptions sessionOptions = new SessionOptions
                {
                    Protocol = Protocol.Sftp,
                    HostName = host,
                    UserName = user,
                    Password = password,
                    SshHostKeyFingerprint = "ssh-rsa 2048 bf:ec:ef:bc:da:98:8e:f2:21:85:76:7c:1a:f8:ed:49"
                };

                using (WinSCP.Session session = new WinSCP.Session())
                {                    
                    session.ExecutablePath = @"E:\Falcon\Tools\Installer\WinSCP.exe";
                    // Connect
                    session.Open(sessionOptions);

                    // Upload files
                    TransferOptions transferOptions = new TransferOptions();
                    transferOptions.FileMask = "| doc/; build_win/; PXTU/";
                    transferOptions.TransferMode = TransferMode.Binary;

                    TransferOperationResult transferResult;                    

                    foreach (Tuple<string, string> entry in toCopy)
                    {
                        //MessageBox.Show("FROM:" + entry.Item1 + "\t TO:" + entry.Item2);                        
                        transferResult = session.PutFiles(entry.Item1, entry.Item2, false, transferOptions);
                        transferResult.Check();
                    }
                }
            }

            catch (Exception x)
            {
                MessageBox.Show(Convert.ToString(x));                
            }
            statusLabel.Text = "Copying Done.";
        }

        private void ssh_command(string command, bool has_return = false)
        {
            using (var client = new SshClient(host, user, password))
            {
                try
                {
                    client.Connect();
                    var output = client.RunCommand(command);
                    //MessageBox.Show(output.Result);                    
                }
                catch (System.Net.Sockets.SocketException)
                {                    
                  MessageBox.Show("Server seems to be down. Please check the connection.");
                }
            }
        }

    }
}
