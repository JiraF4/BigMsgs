// Widget displays msg to player.
// Path: {A5C2415B9B5449A1}UI/layouts/BigMsg/BigMsgBox.layout

class PS_BigMsgBox : SCR_ScriptedWidgetComponent
{
	ImageWidget m_wMsgIcon;
	TextWidget m_wMsgText;
	
	ImageWidget m_wFill;
	ImageWidget m_wEmpty;
	
	float m_fFadeTimeMax = 0.5;
	float m_fShowTimeMax = 0.5;
	
	float m_fFadeTime = 5.0;
	float m_fWholeTime = 5.0;
	
	float m_fLastUpdateTime = 0;
	
	override void HandlerAttached(Widget w)
	{
		if (!GetGame().InPlayMode())
			return;
		
		super.HandlerAttached(w);
		m_wMsgIcon = ImageWidget.Cast(w.FindAnyWidget("MsgIcon"));
		m_wMsgText = TextWidget.Cast(w.FindAnyWidget("MsgText"));
		m_wEmpty = ImageWidget.Cast(w.FindAnyWidget("Fill"));
		m_wFill = ImageWidget.Cast(w.FindAnyWidget("Empty"));
				
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
		
		float percent = m_fFadeTime / (m_fWholeTime - m_fShowTimeMax);
		if (percent < 0)
			percent = 0;
		HorizontalLayoutSlot.SetFillWeight(m_wEmpty, percent);
		HorizontalLayoutSlot.SetFillWeight(m_wFill, 1.0 - percent);
		
		float time = GetGame().GetWorld().GetWorldTime();
		float deltaTime = time - m_fLastUpdateTime;
		m_fLastUpdateTime = time;
		
		m_fFadeTime -= deltaTime / 1000;
		GetGame().GetCallqueue().CallLater(FadeOut, 0, false);
	};
	
	void SetText(string text)
	{
		m_wMsgText.SetText(text);
		
		m_fFadeTime  = text.Length() * 0.08 + 2.5;
		m_fWholeTime = text.Length() * 0.08 + 2.5;
	}
}