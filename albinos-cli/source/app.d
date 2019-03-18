import core.exception : AssertError;
import std.stdio;
import std.string;
import std.getopt;
import std.socket;
import std.uni : isWhite;
import std.process : executeShell;
import std.file;
import asdf;
import protocol.protocol_type;

///
class Client
{
	///
	this(string socket_path)
	{
		this.address_ = new UnixAddress(socket_path);
		this.socket_ = new Socket(AddressFamily.UNIX, SocketType.STREAM);
		socket_.connect(this.address_);
	}

	@property Socket socket() pure nothrow @nogc
	{
		return socket_;
	}

	private UnixAddress address_;
	private Socket socket_;
}

///
class CLI
{
	this(string socket_path)
	{
		try
		{
			client_ = new Client(socket_path);
		}
		catch (SocketOSException e)
		{
			writeln(stderr, e.msg);
			is_running_ = false;
		}
	}

	private config_create_answer create_config(string[] args)
	in
	{
		assert(args !is null, "args cannot be null");
		assert(args.length == 2, "need 1 arguments");
	}
	out(r) 
	{
		assert(r.state == "SUCCESS", "create_config should success");
		assert(!r.config_key.empty, "config_key should not be empty");
		assert(!r.readonly_config_key.empty, "readonly_config_key should not be empty");
	}
	body
	{
		string config_name;
		getopt(args, "name", &config_name);
		

		auto cfg = (cast(const char[])(std.file.read("../API_doc/json_recipes/config_create.json")))
			.deserialize!config_create;
		cfg.config_name = config_name.strip("\"");
		client_.socket.send(cfg.serializeToJson);
		auto answer = new ubyte[256];
		client_.socket.receive(answer);
		client_.socket.getErrorText.writeln;
		return (cast(string)answer).deserialize!config_create_answer;
	}

	///
	public void run_program()
	{
		if (!is_running_)
			return;
		write("> ");
		string line;
		while (((line = stdin.readln().strip("\n \t")) !is null))
		{
			if (line == "exit")
			{
				break;
			}
			auto splited_line = line.split!isWhite;
			const auto cmd = splited_line[0];
			try
			{
				switch (cmd)
				{
				case "create_config":
					this.create_config(splited_line);
					break;
				default:
					line.dup.executeShell.output.write;
					break;
				}
			}
			catch (AssertError e)
			{
				writeln(stderr, e.msg);
			}
			write("> ");
		}
	}

	private Client client_;
	private bool is_running_ = true;
}

void main(string[] args)
{
	writeln(args);
	auto cli = new CLI("/tmp/raven-os_service_albinos.sock");
	cli.run_program;
}
