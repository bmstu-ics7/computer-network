using CommandLine;

namespace Mail
{
    public class Options
    {
        [Option('s', "sender", Required = true, HelpText = "Address of sender.")]
        public string AddressSender { get; set; }

        [Option('r', "recipient", Required = true, HelpText = "Address of recipient.")]
        public string AddressRecipient { get; set; }

        [Option('p', "password", Required = true, HelpText = "Password.")]
        public string Password { get; set; }
    }
}
