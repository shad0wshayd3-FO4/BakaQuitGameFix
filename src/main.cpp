namespace Hooks
{
	class hkQuitGame :
		public REX::Singleton<hkQuitGame>
	{
	private:
		static bool QuitGame()
		{
			if (auto ConsoleLog = RE::ConsoleLog::GetSingleton())
			{
				ConsoleLog->PrintLine("Bye.");
			}

			std::thread(
				[]()
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(2000));
					if (auto Main = RE::Main::GetSingleton())
					{
						Main->quitGame = true;
					}
				})
				.detach();

			return true;
		}

	public:
		static void install()
		{
			static REL::Relocation target{ REL::ID(2205365) };
			target.replace_func(0x29, QuitGame);
		}
	};

	class hkShutdown :
		public REX::Singleton<hkShutdown>
	{
	private:
		static void Shutdown()
		{
			REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), EXIT_SUCCESS);
		}

		inline static REL::Hook Hook{ REL::ID(4812562), 0x20, Shutdown };
	};

	static void Install()
	{
		hkQuitGame::install();
	}
}

namespace
{
	void MessageCallback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kPostLoad:
			Hooks::Install();
			break;
		default:
			break;
		}
	}
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se, { .trampoline = true, .trampolineSize = 14 });
	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	return true;
}
