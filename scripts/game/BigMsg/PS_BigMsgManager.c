//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "", color: "0 0 255 255", icon: HYBRID_COMPONENT_ICON)]
class PS_BigMsgManagerClass: ScriptComponentClass
{
	
};

// Manage Big messages
// Mainly just send info to clients
[ComponentEditorProps(icon: HYBRID_COMPONENT_ICON)]
class PS_BigMsgManager : ScriptComponent
{
	VerticalLayoutWidget m_wBigMsgList;
	
	
	// more singletons for singletons god, make our spagetie kingdom great
	static PS_BigMsgManager GetInstance() 
	{
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (gameMode)
			return PS_BigMsgManager.Cast(gameMode.FindComponent(PS_BigMsgManager));
		else
			return null;
	}
	
	override void OnPostInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(AddMsgAction, 0, false);
		
		Widget widget = GetGame().GetWorkspace().CreateWidgets("{919DD1BCCDC0C57C}UI/layouts/BigMsg/BigMsgList.layout");
		m_wBigMsgList = VerticalLayoutWidget.Cast(widget.FindAnyWidget("BigMsgListVerticalLayout"));
	}
	
	void AddMsgAction()
	{
		SCR_ChatPanelManager chatPanelManager = SCR_ChatPanelManager.GetInstance();
		ChatCommandInvoker invoker = chatPanelManager.GetCommandInvoker("amsg");
		invoker.Insert(SendBigMsg_CallbackAdmin);
		invoker = chatPanelManager.GetCommandInvoker("smsg");
		invoker.Insert(SendBigMsg_CallbackServer);
	}
	
	
	void SendBigMsg_CallbackAdmin(SCR_ChatPanel panel, string data)
	{
		SendBigMsg_Callback(panel, data, PS_EBigMsgType.Admin);
	}
	void SendBigMsg_CallbackServer(SCR_ChatPanel panel, string data)
	{
		if (panel) return;
		SendBigMsg_Callback(panel, data, PS_EBigMsgType.Server);
	}
	
	void SendBigMsg_Callback(SCR_ChatPanel panel, string data, PS_EBigMsgType msgType)
	{
		if (data == "") return;
		PlayerController playerController = GetGame().GetPlayerController();
		PS_BigMsgSenderComponent bigMsgSenderComponent = PS_BigMsgSenderComponent.Cast(playerController.FindComponent(PS_BigMsgSenderComponent));
		bigMsgSenderComponent.SendBigMsg(data, msgType);
	}
	
	void SendMsgToClients(string msg, PS_EBigMsgType msgType)
	{
		RPC_SendMsgToClients(msg, msgType);
		Rpc(RPC_SendMsgToClients, msg, msgType);
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_SendMsgToClients(string msg, PS_EBigMsgType msgType)
	{
		Widget widget;
		switch (msgType)
		{
			case PS_EBigMsgType.Admin:
				widget = GetGame().GetWorkspace().CreateWidgets("{A5C2415B9B5449A1}UI/layouts/BigMsg/BigMsgBox.layout", m_wBigMsgList);
				break;
			case PS_EBigMsgType.Server:
				widget = GetGame().GetWorkspace().CreateWidgets("{A0F64AF249D4819B}UI/layouts/BigMsg/BigMsgBoxServer.layout", m_wBigMsgList);
				break;
		}
		
		PS_BigMsgBox bigMsgBox = PS_BigMsgBox.Cast(widget.FindHandler(PS_BigMsgBox));
		bigMsgBox.SetText(msg);
	}
};