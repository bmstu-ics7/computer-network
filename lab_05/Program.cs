using System;
using CommandLine;
using System.Net;
using System.Net.Mail;
using System.Threading;

namespace Mail
{
    class Program
    {
        static void Main(string[] args)
        {
            CommandLine.Parser.Default.ParseArguments<Options>(args)
                .WithParsed(RunProgram);
        }

        static void RunProgram(Options opts)
        {
            MailAddress sender = new MailAddress(opts.AddressSender, "Alex");
            MailAddress recipient = new MailAddress(opts.AddressRecipient);
            MailMessage m = new MailMessage(sender, recipient);

            Console.Write("Subject: ");
            m.Subject = Console.ReadLine();

            Console.Write("Body: ");
            m.Body = Console.ReadLine();

            Console.Write("Filename [no file]: ");
            string filename = Console.ReadLine();

            if (filename != "")
            {
                m.Attachments.Add(new Attachment(filename));
            }

            Console.WriteLine("Interval: ");
            int interval = Convert.ToInt32(Console.ReadLine());

            SmtpClient smtp = new SmtpClient("smtp.gmail.com", 587);
            smtp.Credentials = new NetworkCredential(opts.AddressSender, opts.Password);
            smtp.EnableSsl = true;

            while (true)
            {
                smtp.Send(m);
                Console.WriteLine("Message sended");
                Thread.Sleep(interval);
            }

        }
    }
}
