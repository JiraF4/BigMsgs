//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Character", description: "Set character playable", color: "0 0 255 255", icon: HYBRID_COMPONENT_ICON)]
class PS_BigMsgSenderComponentClass: ScriptComponentClass
{
	
}

// We can send rpc only from authority
// And here we are, modifying player controller since it's only what we have on client.
[ComponentEditorProps(icon: HYBRID_COMPONENT_ICON)]
class PS_BigMsgSenderComponent : ScriptComponent
{
	void SendBigMsg(string msg, PS_EBigMsgType msgType)
	{
		Rpc(RPC_SendBigMsg, msg, msgType);
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RPC_SendBigMsg(string msg, PS_EBigMsgType msgType)
	{
		PlayerManager playerManager = GetGame().GetPlayerManager();
		PlayerController playerController = PlayerController.Cast(GetOwner());
		if (!playerManager.HasPlayerRole(playerController.GetPlayerId(), EPlayerRole.ADMINISTRATOR))
			return;
		
		PS_BigMsgManager bigMsgManager = PS_BigMsgManager.GetInstance();
		bigMsgManager.SendMsgToClients(msg, msgType);
	}
}