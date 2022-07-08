modded class MissionGameplay
{
	ref Widget m_AdditionHudRootWidget = null;
	ref SyberiaAdditionalHud m_SyberiaAdditionalHud = null;
	
	override void OnMissionStart()
	{
		SybLog("MissionGameplay OnMissionStart");
		super.OnMissionStart();
		SyberiaPPEffects.ResetAllEffects();
	}
	
	override void OnMissionFinish()
	{
		SybLog("MissionGameplay OnMissionStart");
		super.OnMissionFinish();
		
		delete m_AdditionHudRootWidget;
		delete m_SyberiaAdditionalHud;
		SyberiaPPEffects.ResetAllEffects();
	}
	
	override void OnInit()
	{
		if ( m_Initialized )
		{
			return;
		}
		
		super.OnInit();
		
		SyberiaPPEffects.Init();
		
		if (!m_AdditionHudRootWidget)
		{
			m_AdditionHudRootWidget = GetGame().GetWorkspace().CreateWidgets("SyberiaScripts/layout/AdditionalHud.layout");
			m_AdditionHudRootWidget.Show(false);
			
			if ( !m_SyberiaAdditionalHud )
			{
				ref Widget actionBlocker = m_AdditionHudRootWidget.FindAnyWidget("ActionBlocker");
				m_AdditionHudRootWidget.RemoveChild(actionBlocker);
				
				ref MultilineTextWidget screenInfoWidget = MultilineTextWidget.Cast( m_AdditionHudRootWidget.FindAnyWidget("ScreenInfoWidget") );
				m_AdditionHudRootWidget.RemoveChild(screenInfoWidget);
				
				m_SyberiaAdditionalHud = new SyberiaAdditionalHud(m_HudRootWidget, actionBlocker, screenInfoWidget);
				m_SyberiaAdditionalHud.Init();		
			}
			
			if (m_Hud)
			{
				ref Widget notifications = m_AdditionHudRootWidget.FindAnyWidget("Notifications");				
				m_Hud.InitNotifierWidget(NTFKEY_SLEEPING, notifications, "Sleeping");
				
				ref Widget badgets = m_AdditionHudRootWidget.FindAnyWidget("Badgets");
				m_Hud.InitBadgetWidget(NTFKEY_BULLETHIT, badgets, "BulletHit");
				m_Hud.InitBadgetWidget(NTFKEY_KNIFEHIT, badgets, "KnifeHit");
				m_Hud.InitBadgetWidget(NTFKEY_HEMATOMA, badgets, "Hematoma");
				m_Hud.InitBadgetWidget(NTFKEY_VISCERADMG, badgets, "VisceraDamage");
				m_Hud.InitBadgetWidget(NTFKEY_CONCUSSION, badgets, "Concussion");
				m_Hud.InitBadgetWidget(NTFKEY_PAIN, badgets, "Pain");
				m_Hud.InitBadgetWidget(NTFKEY_PAINKILLER, badgets, "Painkiller");
				m_Hud.InitBadgetWidget(NTFKEY_ANTIBIOTIC, badgets, "Antibiotic");
				m_Hud.InitBadgetWidget(NTFKEY_STOMATCHHEAL, badgets, "Stomatchheal");
				m_Hud.InitBadgetWidget(NTFKEY_SEPSIS, badgets, "Sepsis");
				m_Hud.InitBadgetWidget(NTFKEY_ZVIRUS, badgets, "ZVirus");
				m_Hud.InitBadgetWidget(NTFKEY_BANDAGE1, badgets, "Bandage1");
				m_Hud.InitBadgetWidget(NTFKEY_BANDAGE2, badgets, "Bandage2");				
				m_Hud.InitBadgetWidget(NTFKEY_HEMOSTATIC, badgets, "Hemostatic");
				m_Hud.InitBadgetWidget(NTFKEY_HEMATOPOIESIS, badgets, "Hematopoiesis");
				m_Hud.InitBadgetWidget(NTFKEY_USESALVE, badgets, "UseSalve");
				m_Hud.InitBadgetWidget(NTFKEY_ADRENALIN, badgets, "Adrenalin");
				m_Hud.InitBadgetWidget(NTFKEY_OVERDOSED, badgets, "Overdosed");
			}
		}
		
		GetSyberiaRPC().RegisterHandler(SyberiaRPC.SYBRPC_SCREEN_MESSAGE, this, "OnScreenMessageRpc");
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
				
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if (player)
		{
			SyberiaPPEffects.Update(timeslice);
			
			UIScriptedMenu menu = m_UIManager.GetMenu();
			
			if (m_SyberiaAdditionalHud && m_LifeState == EPlayerStates.ALIVE && !player.IsUnconscious() )
			{
				m_SyberiaAdditionalHud.Refresh(timeslice);	
			}
			
			if (m_Hud)
			{
				m_Hud.DisplayNotifier(NTFKEY_SLEEPING, player.GetSleepingTendency(), player.GetSleepingState());
				m_Hud.DisplayBadge(NTFKEY_BULLETHIT, player.m_bulletHits);
				m_Hud.DisplayBadge(NTFKEY_KNIFEHIT, player.m_knifeHits);
				m_Hud.DisplayBadge(NTFKEY_HEMATOMA, player.m_hematomaHits);
				m_Hud.DisplayBadge(NTFKEY_VISCERADMG, player.m_visceraHit);
				m_Hud.DisplayBadge(NTFKEY_CONCUSSION, player.m_concussionHit);
				m_Hud.DisplayBadge(NTFKEY_PAIN, player.GetCurrentPainLevel());
				m_Hud.DisplayBadge(NTFKEY_PAINKILLER, player.m_painkillerEffect);
				m_Hud.DisplayBadge(NTFKEY_ANTIBIOTIC, player.m_antibioticsEffect);
				m_Hud.DisplayBadge(NTFKEY_STOMATCHHEAL, player.m_stomatchhealEffect);
				m_Hud.DisplayBadge(NTFKEY_SEPSIS, player.HasVisibleSepsis());
				m_Hud.DisplayBadge(NTFKEY_ZVIRUS, player.HasVisibleZVirus());
				m_Hud.DisplayBadge(NTFKEY_BANDAGE1, player.m_bulletBandage1 + player.m_knifeBandage1);
				m_Hud.DisplayBadge(NTFKEY_BANDAGE2, player.m_bulletBandage2 + player.m_knifeBandage2);
				m_Hud.DisplayBadge(NTFKEY_HEMOSTATIC, player.m_bloodHemostaticEffect);
				m_Hud.DisplayBadge(NTFKEY_HEMATOPOIESIS, player.m_hematopoiesisEffect);
				m_Hud.DisplayBadge(NTFKEY_USESALVE, player.m_salveEffect);
				m_Hud.DisplayBadge(NTFKEY_ADRENALIN, player.m_adrenalinEffect);
				m_Hud.DisplayBadge(NTFKEY_OVERDOSED, (int)Math.Floor(Math.Clamp(player.m_overdosedValue, 0, 3)));
			}
						
			OnUpdateAdvMedicineGUI(player, timeslice);
		}
		
		if (!player || !player.IsAlive())
		{
			if (m_HudRootWidget)
			{
				m_HudRootWidget.Show(false);
			}
		}
	}
	
	private void OnUpdateAdvMedicineGUI(PlayerBase player, float deltaTime)
	{		
		float overdosedEffect = Math.Clamp((player.m_overdosedValue - 1.0) * 0.1, 0, 0.3);
		SyberiaPPEffects.SetOverdosedEffect(overdosedEffect);
		
		float painEffect = Math.Clamp(player.GetCurrentPainLevel() * 0.1, 0, 0.3);
		SyberiaPPEffects.SetPainEffect(painEffect);
		
		float concussionEffect = Math.Clamp(((int)player.m_concussionHit) * 0.1, 0, 0.1);
		SyberiaPPEffects.SetConcussionEffect(concussionEffect);
	}
	
	override int GetRespawnModeClient()
	{
		return GameConstants.RESPAWN_MODE_RANDOM;
	}
	
	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
		
		PluginGearPDA pluginGearPDA;
		if ( key == KeyCode.KC_ESCAPE )
		{	
			Class.CastTo(pluginGearPDA, GetPlugin(PluginGearPDA));
			if (pluginGearPDA && pluginGearPDA.IsOpen())
			{
				pluginGearPDA.Close();
			}
		}
		else if ( key == KeyCode.KC_RETURN )
		{
			Class.CastTo(pluginGearPDA, GetPlugin(PluginGearPDA));
			if (pluginGearPDA && pluginGearPDA.IsOpen())
			{
				pluginGearPDA.m_GearPDAMenu.m_externalSendEvent = true;
			}
		}
	}

	protected void OnScreenMessageRpc(ref ParamsReadContext ctx, ref PlayerIdentity sender)
	{
		if (!m_SyberiaAdditionalHud) return;
		
		Param1<string> clientData;
       	if ( !ctx.Read( clientData ) ) return;	
		
		m_SyberiaAdditionalHud.ShowScreenMessage(clientData.param1, 8);
	}
};