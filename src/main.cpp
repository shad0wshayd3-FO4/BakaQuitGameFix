namespace Hook
{
	class hkShutdown :
		public REX::Singleton<hkShutdown>
	{
	private:
		static void Shutdown()
		{
			REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), EXIT_SUCCESS);
		}

		inline static REL::Hook Hook{ REL::Offset(0x0C2A410), 0x20, Shutdown };
	};
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se, { .trampoline = true });
	return true;
}
