// Widget displays msg to player.
// Path: {A5C2415B9B5449A1}UI/layouts/BigMsg/BigMsgBox.layout

class PS_BigMsgBox : SCR_ScriptedWidgetComponent
{
	
	ImageWidget m_wMsgIcon;
	TextWidget m_wMsgText;
	
	float m_fFadeTimeMax = 0.5;
	float m_fShowTimeMax = 0.5;
	
	float m_fFadeTime = 5.0;
	float m_fWholeTime = 5.0;
	
	float m_fLastUpdateTime = 0;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_wMsgIcon = ImageWidget.Cast(w.FindAnyWidget("MsgIcon"));
		m_wMsgText = TextWidget.Cast(w.FindAnyWidget("MsgText"));
		super.HandlerAttached(w);
		
		AudioSystem.PlaySound("{06C02AFB2CA882EB}Sounds/UI/Samples/Menu/UI_Task_Created.wav");
		
		if (GetGame().InPlayMode())
			m_wRoot.SetOpacity(0);
		
		m_fLastUpdateTime = GetGame().GetWorld().GetWorldTime();
		
		GetGame().GetCallqueue().CallLater(FadeOut, 0, false);
	}
	
	void FadeOut()
	{
		float opacity = m_fFadeTime / m_fFadeTimeMax;
		float showTime = (m_fWholeTime - m_fShowTimeMax);
		if (m_fFadeTime > showTime) opacity = 1.0 - (m_fFadeTime - showTime) / m_fShowTimeMax;
		if (m_fFadeTime <= 0)
		{
			m_wRoot.RemoveFromHierarchy();
			return;
		}
		m_wRoot.SetOpacity(opacity);
		
		
		float time = GetGame().GetWorld().GetWorldTime();
		float deltaTime = time - m_fLastUpdateTime;
		m_fLastUpdateTime = time;
		
		m_fFadeTime -= deltaTime / 1000;
		GetGame().GetCallqueue().CallLater(FadeOut, 0, false);
	};
	
	void SetText(string text)
	{
		m_wMsgText.SetText(text);
	}
}