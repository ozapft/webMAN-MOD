/*
 FAIL SAFE    : SELECT+L3+L2+R2
 RESET SAFE   : SELECT+R3+L2+R2

 REFRESH XML  : SELECT+L3 (+R2=profile1, +L2=profile2, +R1=profile3, +L1=profile4, +L1+R1=Reload XMB, +R2+L2=FAIL SAFE)
                                                *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_l3
 UNLOAD WM    : R2+L3+R3 / L3+R3+R2 / L3+R2+R3

 GOTO_HOME    : L2+L3+R3 / L3+R3+L2 / L3+L2+R3  *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l3_r3_l2

 PLAY_DISC    : L2+START                        *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_start
 PLAY APP_HOME: R2+START                        *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_r2_start

 PREV GAME    : SELECT+L1                       *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_l1
 NEXT GAME    : SELECT+R1                       *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_r1
 UMNT_GAME    : SELECT+O (unmount)              *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_circle

 RESERVED     : SELECT+TRIANGLE                 *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_triangle
 RESERVED     : SELECT+CROSS                    *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_cross

 EXT GAME DATA: SELECT+□                        *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_square
 MOUNT net0/  : SELECT+R2+□
 MOUNT net1/  : SELECT+L2+□

 SHUTDOWN     : L3+R2+X
 SHUTDOWN  *2 : L3+R1+X (vsh shutdown) <- alternative shutdown method
 RESTART      : L3+R2+O (lpar restart)
 RESTART   *2 : L3+R1+O (vsh restart)  <- alternative restart method

 FAN CNTRL    : L3+R2+START  (enable/disable fancontrol)
 SHOW TEMP    : SELECT+START (SELECT+START+R2 will show only copy progress) / SELECT+R3 (if rec video flag is disabled)

 DYNAMIC TEMP : SELECT+LEFT/RIGHT               *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_left
                                                *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_right
 MANUAL TEMP  : SELECT+UP/DOWN                  *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_up
                                                *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_down

 REC VIDEO    : SELECT+R3          Record video using internal plugin (IN-GAME ONLY)
                                   *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_r3
 REC VIDEO PLG: SELECT+R3+L2+R2    Unload webMAN & Record video with video_rec plugin (IN-GAME ONLY)
 REC VIDEO SET: SELECT+R3+L2       Select video rec setting
 REC VIDEO VAL: SELECT+R3+R2       Change value of video rec setting
 XMB SCRNSHOT : L2+R2+SELECT+START              *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_r2_select_start

 USER/ADMIN   : L2+R2+TRIANGLE                  *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_r2_triangle

 SYSCALLS     : R2+TRIANGLE                     *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_r2_triangle
 SHOW IDPS    : R2+O  (Abort copy/fix process)  *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_r2_circle
 OFFLINE MODE : R2+□                            *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_r2_square

 QUICK INSTALL: SELECT+R2+O                     *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_select_r2_circle

 TOGGLE PS2CLASSIC    : SELECT+L2+TRIANGLE
 SWITCH PS2EMU        : SELECT+L2+R2

 COBRA TOGGLE         : L3+L2+TRIANGLE
 REBUG  Mode Switcher : L3+L2+□
 Normal Mode Switcher : L3+L2+O
 DEBUG  Menu Switcher : L3+L2+X

 SKIP AUTO-MOUNT   : L2+R2  (at startup only)   *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_r2

 VSH MENU          : SELECT (hold down for few seconds on XMB only)
 sLaunch MENU      : L2+R2 or START (hold down for few seconds on XMB only)

 Open File Manager : L2+R2+O                    *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_r2_circle
 Open Games List   : L2+R2+R1+O                 *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_r2_r1_circle
 Open System Info  : L2+R2+L1+O                 *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_r2_l1_circle
 Open Setup        : L2+R2+L1+R1+O              *or* Custom Combo -> /dev_hdd0/tmp/wm_combo/wm_custom_l2_r2_l1_r1_circle
*/
		bool reboot = false;

		u8 n;

		init_delay = 0;

		CellPadInfo2 padinfo;

		#define PERSIST  248

		for(n = 0; n < 10; n++)
		{
			if(!working) break;

			if(show_persistent_popup == PERSIST) {goto show_persistent_popup;}
			if(show_info_popup) {show_info_popup = false; goto show_popup;}

			//if(!webman_config->nopad)
			{
				pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] = pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] = pad_data.len = 0;

#ifdef VIRTUAL_PAD
				if(vcombo)
				{
					pad_data.len = 16; pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] = (vcombo & 0xFF); pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] = (vcombo & 0xFF00) >> 8; vcombo = 0;
				}
				else
#endif
				if(cellPadGetInfo2(&padinfo) == CELL_OK)
				{
					for(u8 p = 0; p < 8; p++)
						if((padinfo.port_status[p] == CELL_PAD_STATUS_CONNECTED) && (cellPadGetData(p, &pad_data) == CELL_PAD_OK) && (pad_data.len > 0)) break;
				}

				if(pad_data.len > 0)
				{
#ifdef COBRA_ONLY
					if( ((!(webman_config->combo2 & C_SLAUNCH)) && (((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_START) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == 0)) ||                    // START  = SLAUNCH MENU
																	((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == 0) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2)))))   // L2+R2  = SLAUNCH MENU
					||	((!(webman_config->combo2 & C_VSHMENU)) &&  ((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_SELECT) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == 0))) )                   // SELECT = VSH MENU
					{
#ifdef WM_CUSTOM_COMBO
						if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2)) // L2+R2
						{
							if(do_custom_combo("l2_r2")) continue;
						}
#endif
						if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] && (++init_delay < 5)) {sys_ppu_thread_usleep(100000); continue;}

						start_vsh_gui(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_SELECT);

						while(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] | pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2]) pad_data = pad_read();
						break;
					}
#endif
					if(!(webman_config->combo2 & PLAY_DISC) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_START))
					{
						// L2+START = Play Disc
						// R2+START = Play app_home/PS3_GAME
						if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_L2)
						{
#ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("l2_start")) continue;
#endif
							launch_disc(true); // L2+START
							break;
						}
#ifdef COBRA_ONLY
						if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R2)
						{
#ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("r2_start")) continue;
#endif
							if(not_exists("/app_home/PS3_GAME/USRDIR/EBOOT.BIN"))
							{
								if(is_app_dir(webman_config->home_url, "."))
									set_app_home(webman_config->home_url);
								else if(islike(webman_config->home_url, "http"))
									open_browser(webman_config->home_url, 0);
#ifdef WM_REQUEST
								else if(*webman_config->home_url == '/') handle_file_request(webman_config->home_url); // web command
#endif
							}

							launch_app_home_icon(true);
							break;
						}
#endif
					}

					if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_SELECT))
					{
						if( !(webman_config->combo & FAIL_SAFE) &&
							(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_L3)) && // fail-safe mode
							(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2))        // SELECT+L3+L2+R2
							)
						{
							//// startup time /////
							CellRtcTick pTick; cellRtcGetCurrentTick(&pTick);
							u32 ss = (u32)((pTick.tick - rTick.tick)/1000000);
							///////////////////////

							if(!sys_admin || (ss > 60) || IS_INGAME) continue; // allow delete boot_plugins.txt only on XMB to sys_admin in the first minute after boot

							cellFsUnlink("/dev_hdd0/boot_plugins.txt");
							cellFsUnlink("/dev_hdd0/boot_plugins_nocobra.txt");
							cellFsUnlink("/dev_hdd0/boot_plugins_nocobra_dex.txt");
							goto reboot; // vsh reboot
						}
						else
						if( (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_R3)) &&       // reset-safe mode
							(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2)) && IS_ON_XMB // SELECT+R3+L2+R2
							)
						{
							if(!sys_admin || IS_INGAME) continue; // allow reset config only for sys_admin on XMB
 #ifndef ENGLISH_ONLY
							char STR_RMVWMCFG[96];//	= "webMAN config reset in progress...";
							char STR_RMVWMCFGOK[112];//	= "Done! Restart within 3 seconds";

							language("STR_RMVWMCFG", STR_RMVWMCFG, WM_APPNAME " config reset in progress...");
							language("STR_RMVWMCFGOK", STR_RMVWMCFGOK, "Done! Restart within 3 seconds");

							close_language();
 #endif
							cellFsUnlink(WMCONFIG);
							{ BEEP1 }
							show_msg(STR_RMVWMCFG);
							sys_ppu_thread_sleep(2);
							show_msg(STR_RMVWMCFGOK);
							sys_ppu_thread_sleep(3);
							goto reboot; // vsh reboot
						}
 #ifdef COBRA_ONLY
  #ifndef LITE_EDITION
						else
						if( !(webman_config->combo2 & PS2TOGGLE)
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_L2)
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_TRIANGLE) // SELECT+L2+TRIANGLE
							&& (c_firmware >= 4.65f) )
						{
							bool classic_ps2_enabled = file_exists(PS2_CLASSIC_TOGGLER);

							if(classic_ps2_enabled)
							{
								disable_classic_ps2_mode();
							}
							else
							{
								enable_classic_ps2_mode();
							}

							show_status("PS2 Classic", classic_ps2_enabled ? STR_DISABLED : STR_ENABLED);

							n = 0;
							break;
						}
						else
						if( !(webman_config->combo2 & PS2SWITCH)
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_L2) // Clone ps2emu habib's switcher
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2) // SELECT+L2+R2
							&& (c_firmware>=4.53f) )
						{
								toggle_ps2emu();
						}
  #endif //#ifndef LITE_EDITION
 #endif //#ifdef COBRA_ONLY
						else
						if(!(webman_config->combo2 & XMLREFRSH) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_L3))) // SELECT+L3 refresh XML
						{
							// SELECT+L3       = refresh XML
							// SELECT+L3+R2    = refresh XML profile 1
							// SELECT+L3+L2    = refresh XML profile 2
							// SELECT+L3+R1    = refresh XML profile 3
							// SELECT+L3+L1    = refresh XML profile 4
							// SELECT+L3+L1+R1 = refresh XML + Reload XMB
 #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("select_l3")) break;
 #endif

							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R2) profile = 1; else
							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_L2) profile = 2; else
							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R1) profile = 3; else
							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_L1) profile = 4; else profile = 0;

							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R1 | CELL_PAD_CTRL_L1)) n = 11;

							refresh_xml(msg);
							if(n > 10) reload_xmb();
						}
 #ifdef VIDEO_REC
						else
						if(!(webman_config->combo2 & VIDRECORD) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_R3))) // SELECT + R3
						{
							// SELECT+R3+L2+R2  = Record video with video_rec plugin (IN-GAME ONLY)
							// SELECT+R3+L2     = Select video rec setting
							// SELECT+R3+R2     = Change value of video rec setting
							// SELECT+R3        = Toggle Record Video
  #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("select_r3")) break;
							else
  #endif
  #ifdef COBRA_ONLY
							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2)) // SELECT+R3+L2+R2  Record video with video_rec plugin (IN-GAME ONLY)
							{
								// SELECT+R3+L2+R2 = Record video with video_rec plugin (IN-GAME ONLY)

								#define VIDEO_REC_PLUGIN  WM_RES_PATH "/video_rec.sprx"

								if((!recording) && (IS_INGAME) && file_exists(VIDEO_REC_PLUGIN))
								{
									unsigned int slot = get_free_slot();
									if((slot < 7) && cobra_load_vsh_plugin(slot, VIDEO_REC_PLUGIN, NULL, 0) == CELL_OK) {sys_ppu_thread_sleep(3); goto quit_plugin;} // unload webMAN to free resources
								}
							}
							else
  #endif //#ifdef COBRA_ONLY
							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_L2)	// SELECT+R3+L2  Select video rec setting
							{
								// SELECT+R3+L2 = Select video rec setting

								rec_setting_to_change++; if(rec_setting_to_change>5) rec_setting_to_change = 0;
								set_setting_to_change(msg, "Change : ");

								strcat(msg, "\n\nCurrent recording format:");
								show_rec_format(msg);
							}
							else
							if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R2)	// SELECT+R3+R2  Change value of video rec setting
							{
								// SELECT+R3+R2 = Change value of video rec setting

								set_setting_to_change(msg, "Changed : ");

								strcat(msg, "\n\nCurrent recording format:");
								if(rec_setting_to_change == 0)
								{
									rec_audio_format = CELL_REC_PARAM_AUDIO_FMT_AAC_64K;
									if(rec_video_format == CELL_REC_PARAM_VIDEO_FMT_M4HD_HD720_5000K_30FPS)   {rec_video_format = CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_2048K_30FPS; } else
									if(rec_video_format == CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_2048K_30FPS)  {rec_video_format = CELL_REC_PARAM_VIDEO_FMT_AVC_MP_MIDDLE_768K_30FPS; rec_audio_format = CELL_REC_PARAM_AUDIO_FMT_PCM_768K;} else
									if(rec_video_format == CELL_REC_PARAM_VIDEO_FMT_AVC_MP_MIDDLE_768K_30FPS) {rec_video_format = CELL_REC_PARAM_VIDEO_FMT_MPEG4_SMALL_512K_30FPS;  } else
									if(rec_video_format == CELL_REC_PARAM_VIDEO_FMT_MPEG4_SMALL_512K_30FPS)   {rec_video_format = CELL_REC_PARAM_VIDEO_FMT_MJPEG_HD720_11000K_30FPS; rec_audio_format = CELL_REC_PARAM_AUDIO_FMT_AAC_96K;} else
																											  {rec_video_format = CELL_REC_PARAM_VIDEO_FMT_M4HD_HD720_5000K_30FPS;  }

									show_rec_format(msg);
								}
								if(rec_setting_to_change == 1) {rec_video_format += 0x1000; if((rec_video_format & 0xF000) > 0x4000) rec_video_format &= 0x0FFF;} else
								if(rec_setting_to_change == 2) {rec_video_format += 0x0100; if((rec_video_format & 0x0F00) > 0x0300) {rec_video_format += 0x0200; if((rec_video_format & 0x0F00) > 0x0600) rec_video_format &= 0xF0FF;}} else
								if(rec_setting_to_change == 3) {rec_video_format += 0x0010; if((rec_video_format & 0x00F0) > 0x0090) rec_video_format &= 0xFF0F; else if((rec_video_format & 0x00F0) == 0x0050) rec_video_format += 0x0010;} else
								if(rec_setting_to_change == 4) {rec_audio_format += 0x1000; if((rec_audio_format & 0xF000) > 0x2000) rec_audio_format &= 0x0FFF;} else
								if(rec_setting_to_change == 5) {rec_audio_format += 0x0001; if((rec_audio_format & 0x000F) > 0x0002) {rec_audio_format += 0x0004; if((rec_audio_format & 0x000F) > 0x0009) rec_audio_format &= 0xFFF0;}}

								show_rec_format(msg);
							}
							else
								{memset(msg, 0, sizeof(msg)); toggle_video_rec(msg); n = 0;} // SELECT+R3  Record Video

							break;
						}
 #endif
						else
						if( !(webman_config->combo & SHOW_TEMP) && ((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_START)) // SELECT+START show temperatures / hdd space
 #ifndef VIDEO_REC
																||  (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_R3 | CELL_PAD_CTRL_START))
 #endif
							))
						{
							// SELECT+START+L2+R2 = screenshot of XMB
							// SELECT+START       = show temp or copy progress + show temp (hold SELECT+START for 5 seconds to toggle persistent popup)
							// SELECT+START+R2    = show only copy progress
							// SELECT+R3          = show temp (if no VIDEO_REC)
 #ifdef XMB_SCREENSHOT
							if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_L2)) && IS_ON_XMB)
							{
  #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("l2_r2_select_start")) break;
							else
  #endif
								{BEEP2; memset(msg, 0, sizeof(msg)); saveBMP(msg, true, false); n = 0; break;} // L2 + R2 + SELECT + START
							}
							else
 #endif
							{
								if(show_persistent_popup == 0)        show_persistent_popup = 1;               else
								if(show_persistent_popup  < PERSIST) {BEEP1; show_persistent_popup = PERSIST;} else
																	 {BEEP2; show_persistent_popup = 0;}
 show_persistent_popup:
								/////////////////////////////
 #if defined(FIX_GAME) || defined(COPY_PS3)
								if(copy_in_progress || fix_in_progress)
								{
  #ifdef FIX_GAME
									if(fix_in_progress)
										sprintf(msg, "%s %s", STR_FIXING, current_file);
									else
  #endif
										sprintf(msg, "%s %s (%i %s)", STR_COPYING, current_file, copied_count, STR_FILES);

									show_msg(msg);
									sys_ppu_thread_sleep(2);
									if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_L2) ) break;
								}
 #endif
								/////////////////////////////
 show_popup:
								{ PS3MAPI_ENABLE_ACCESS_SYSCALL8 }

								CellRtcTick pTick; u32 dd, hh, mm, ss;

								cellRtcGetCurrentTick(&pTick);

								{
									u8 st, mode, unknown;
									sys_sm_get_fan_policy(0, &st, &mode, &fan_speed, &unknown);
								}

								_meminfo meminfo;
								{system_call_1(SC_GET_FREE_MEM, (u64)(u32) &meminfo);}

								// detect aprox. time when a game is launched
								poll_start_play_time();

								bool R2 = (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R2), bb;

								///// startup/play time /////
								bb = (!R2 && gTick.tick > rTick.tick); // show play time
								ss = (u32)((pTick.tick - (bb ? gTick.tick : rTick.tick)) / 1000000); dd = (u32)(ss / 86400);
								if(dd > 100) {bb = false; ss = (u32)((pTick.tick-rTick.tick)/1000000); dd = (u32)(ss / 86400);}
								ss %= 86400; hh = (u32)(ss / 3600); ss %= 3600; mm = (u32)(ss / 60); ss %= 60;
								/////////////////////////////

								char net_type[8] = "", ip[ip_size] = "-";
								get_net_info(net_type, ip);

								char cfw_info[24];
								get_cobra_version(cfw_info);

								char smax[24];
								if(fan_ps2_mode || ps2_classic_mounted)
									sprintf(smax, "   PS2 Mode");
								else if(webman_config->fanc == FAN_AUTO2)
									sprintf(smax, "   MAX: AUTO");
								else if(max_temp)
									sprintf(smax, "   MAX: %i°C", max_temp);
								else if(webman_config->fanc == DISABLED)
									sprintf(smax, "   SYSCON");
								else
									memset(smax, 0, sizeof(smax));

								get_temperature(0, &t1); // CPU
								get_temperature(1, &t2); // RSX

								char *RSX = (char*)" RSX:";
								if(webman_config->lang == 99) RSX = (char*)"/";

								char days[6]; *days = NULL;
								if(dd) sprintf(days, "%id ", dd);

								syscalls_removed = CFW_SYSCALLS_REMOVED(TOC);
								if(!syscalls_removed) disable_signin_dialog();

								u16 len =
								sprintf(msg, "CPU: %i°C %s %i°C  FAN: %i%%   \n"
											 "%s: %s%02d:%02d:%02d%s\n"
											 "%s : %s %s\n"
											 "IP: %s  %s  %s\n",
											 t1, RSX, t2, (int)(((int)fan_speed*100)/255),
											 bb ? "Play" : "Startup", days, hh, mm, ss, smax,
											 STR_FIRMWARE, fw_version, cfw_info, ip, net_type, syscalls_removed ? "[noSC]" :
												  (webman_config->combo & SYS_ADMIN) ? (sys_admin ? "[ADMIN]":"[USER]") : "");

								if(R2 && (gTick.tick > rTick.tick))
								{
									////// play time //////
									ss = (u32)((pTick.tick-gTick.tick)/1000000);
									dd = (u32)(ss / 86400); ss %= 86400; hh = (u32)(ss / 3600); ss %= 3600; mm = (u32)(ss / 60); ss %= 60;

									if(dd < 100)
									{
										char tmp[202]; get_game_info();
										*days = NULL; if(dd) sprintf(days, "%id ", dd);
										snprintf(tmp, 200,  "%s %s\n\n"
															"Play: %s%02d:%02d:%02d\n"
															"%s", _game_TitleID, _game_Title, days, hh, mm, ss, msg);
										strcpy(msg, tmp);
									}
								}
								else
								{
									char hdd_free[40];
									free_size(drives[0], hdd_free);
									int mem_free = (int)(meminfo.avail>>10);
									sprintf(msg + len,  "%s: %s\n"
														"%s: %i %s\n",
														STR_STORAGE, hdd_free,
														STR_MEMORY,  mem_free, STR_KBFREE);
								}

								{ PS3MAPI_DISABLE_ACCESS_SYSCALL8 }

								show_msg(msg);
								sys_ppu_thread_sleep(2);
							}
						}
						else
						if(webman_config->fanc && !(webman_config->combo & MANUALFAN) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_UP))) // SELECT+UP increase TEMP/FAN
						{
							// SELECT+UP      = increase TEMP of dynamic fan control / manual FAN SPEED +1
							// SELECT+UP + R2 = increase TEMP of dynamic fan control / manual FAN SPEED +5
 #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("select_up")) break;
							else
 #endif
							{
								if(webman_config->fanc == DISABLED) enable_fan_control(ENABLE_SC8);

								if(max_temp) //auto mode
								{
									if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2) max_temp+=5; else max_temp+=1;
									if(max_temp > MAX_TEMPERATURE) max_temp = MAX_TEMPERATURE;
									webman_config->dyn_temp = max_temp;
									sprintf(msg, "%s\n%s %i°C", STR_FANCH0, STR_FANCH1, max_temp);
								}
								else
								{
									if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2) webman_config->man_rate += 5; else webman_config->man_rate += 1;
									webman_config->man_rate = RANGE(webman_config->man_rate, 20, 95); //%
									webman_config->man_speed = PERCENT_TO_8BIT(webman_config->man_rate);
									webman_config->man_speed = RANGE(webman_config->man_speed, MIN_FANSPEED_8BIT, MAX_FANSPEED_8BIT);
									set_fan_speed(webman_config->man_speed + 1);
									sprintf(msg, "%s\n%s %i%%", STR_FANCH0, STR_FANCH2, webman_config->man_rate);
								}
								save_settings();
								show_msg(msg);

								n = 0;
								break;
							}
						}
						else
						if(webman_config->fanc && !(webman_config->combo & MANUALFAN) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_DOWN))) // SELECT+DOWN decrease TEMP/FAN
						{
							// SELECT+DOWN    = decrease TEMP of dynamic fan control / manual FAN SPEED -1
							// SELECT+DOWN+R2 = decrease TEMP of dynamic fan control / manual FAN SPEED -5
 #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("select_down")) break;
							else
 #endif
							{
								if(webman_config->fanc == DISABLED) enable_fan_control(ENABLE_SC8);

								if(max_temp) //auto mode
								{
									if(max_temp > 30) {if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2) max_temp-=5; else max_temp-=1;}
									webman_config->dyn_temp = max_temp;
									sprintf(msg, "%s\n%s %i°C", STR_FANCH0, STR_FANCH1, max_temp);
								}
								else
								{
									if(webman_config->man_rate>20) {if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2) webman_config->man_rate -= 5; else webman_config->man_rate -= 1;}
									webman_config->man_speed = PERCENT_TO_8BIT(webman_config->man_rate);
									webman_config->man_speed = RANGE(webman_config->man_speed, MIN_FANSPEED_8BIT, MAX_FANSPEED_8BIT);
									set_fan_speed(webman_config->man_speed + 1);
									sprintf(msg, "%s\n%s %i%%", STR_FANCH0, STR_FANCH2, webman_config->man_rate);
								}
								save_settings();
								show_msg(msg);

								n = 0;
								break;
							}
						}
						else
						if(webman_config->minfan && !(webman_config->combo & MINDYNFAN) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_LEFT))) // SELECT+LEFT decrease Minfan
						{
							// SELECT+LEFT = decrease Minfan
 #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("select_left")) break;
							else
 #endif
							{
								if(webman_config->fanc == DISABLED) enable_fan_control(ENABLE_SC8);

								if(webman_config->minfan-5 >= MIN_FANSPEED) webman_config->minfan -= 5;
								sprintf(msg, "%s\n%s %i%%", STR_FANCH0, STR_FANCH3, webman_config->minfan);

								save_settings();
								show_msg(msg);

								n = 0;
								break;
							}
						}
						else
						if(webman_config->minfan && !(webman_config->combo & MINDYNFAN) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_SELECT | CELL_PAD_CTRL_RIGHT))) // SELECT+RIGHT increase Minfan
						{
							// SELECT+RIGHT = increase Minfan
 #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("select_right")) break;
							else
 #endif
							{
								if(webman_config->fanc == DISABLED) enable_fan_control(ENABLE_SC8);

								if(webman_config->minfan < 95) webman_config->minfan += 5;
								sprintf(msg, "%s\n%s %i%%", STR_FANCH0, STR_FANCH3, webman_config->minfan);

								save_settings();
								show_msg(msg);

								n = 0;
								break;
							}
						}
						else if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_SELECT)
						{
							// SELECT+SQUARE    = Toggle External Game Data
							// SELECT+SQUARE+R2 = Mount net0
							// SELECT+SQUARE+L2 = Mount net1

							// SELECT+L1 = mount previous title
							// SELECT+R1 = mount next title
							// SELECT+O+R2 = Install PKG
							// SELECT+O = unmount
							// SELECT+TRIANGLE = RESERVED

							if( !(webman_config->combo2 & (EXTGAMDAT | MOUNTNET0 | MOUNTNET1)) &&       // Toggle External Game Data
								(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_SQUARE)) // SELECT+SQUARE
							{
 #ifdef COBRA_ONLY
  #ifndef LITE_EDITION
								if(!(webman_config->combo2 & MOUNTNET0) &&
									(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_SQUARE | CELL_PAD_CTRL_R2)))
								{if(is_netsrv_enabled(0)) mount_game("/net0", EXPLORE_CLOSE_ALL);} // SELECT+SQUARE+R2 / SELECT+R2+SQUARE
								else
								if(!(webman_config->combo2 & MOUNTNET1) &&
									(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_SQUARE | CELL_PAD_CTRL_L2)))
								{if(is_netsrv_enabled(1)) mount_game("/net1", EXPLORE_CLOSE_ALL);} // SELECT+SQUARE+L2 / SELECT+L2+SQUARE
								else if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_SQUARE)
  #endif
 #endif
								{
 #ifdef WM_CUSTOM_COMBO
									if(do_custom_combo("select_square")) break;
									else
 #endif
									{
 #ifdef EXT_GDATA
										if((extgd == 0) && isDir("/dev_bdvd/GAMEI"))
											set_gamedata_status(2, true); // enable external gameDATA (if GAMEI exists on /bdvd)
										else
											set_gamedata_status(extgd^1, true); // SELECT+SQUARE
 #endif
										n = 0;
										break;
									}
								}
							}
							else
							if(!(webman_config->combo & PREV_GAME) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_L1) ) // SELECT+L1 (previous title)
							{
								 // SELECT+L1 = mount previous title
 #ifdef WM_CUSTOM_COMBO
								if(do_custom_combo("select_l1")) break;
								else
 #endif
								{
									mount_game("_prev", EXPLORE_CLOSE_ALL);

									n = 0;
									break;
								}
							}
							else
							if(!(webman_config->combo & NEXT_GAME) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R1) ) // SELECT+R1 (next title)
							{
								// SELECT+R1 = mount next title
 #ifdef WM_CUSTOM_COMBO
								if(do_custom_combo("select_r1")) break;
								else
 #endif
								{
									mount_game("_next", EXPLORE_CLOSE_ALL);

									n = 0;
									break;
								}
							}
 #ifdef PKG_HANDLER
							else
							if(!(webman_config->combo2 & INSTALPKG) && ( pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CIRCLE_BTN | CELL_PAD_CTRL_R2) ))  // SELECT+R2+O (Install PKG)
							{
								// SELECT+R2+O = Install PKG
								// SELECT+R2+X = Install PKG (JAP)
  #ifdef WM_CUSTOM_COMBO
								if(do_custom_combo("select_r2_circle")) break;
								else
  #endif
								installPKG_all(DEFAULT_PKG_PATH, true);

								n = 0;
								break;
							}
 #endif //#ifdef PKG_HANDLER
							else
							if(!(webman_config->combo2 & UMNT_GAME) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CIRCLE_BTN)) // SELECT+O (unmount)
							{
								// SELECT+O = unmount
								// SELECT+X = unmount (JAP)
 #ifdef WM_CUSTOM_COMBO
								if(do_custom_combo("select_circle")) break;
								else
 #endif
 #ifdef ALLOW_DISABLE_MAP_PATH
								{
									u64 open_hook_symbol = (dex_mode) ? open_hook_dex : open_hook_cex;

									bool map_path_enabled = (peekq(open_hook_symbol) != original_open_hook);

									do_umount(true);

									if(webman_config->app_home)
									{
										show_status("map_path", disable_map_path(map_path_enabled) ? STR_DISABLED : STR_ENABLED);
									}
								}
 #else
									do_umount(true);
 #endif

								n = 0;
								break;
							}
 #ifdef WM_CUSTOM_COMBO
							else
							if(!(webman_config->combo2 & UMNT_GAME) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_TRIANGLE) ) // SELECT+TRIANGLE
							{
								// SELECT+TRIANGLE = RESERVED for custom combos

								//if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_TRIANGLE)) && do_custom_combo("select_l2_triangle")) break; // RESERVED
								//if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_TRIANGLE)) && do_custom_combo("select_r2_triangle")) break; // RESERVED
								if(do_custom_combo("select_triangle")) break;    // RESERVED
							}
							else
							if(!(webman_config->combo2 & UMNT_GAME) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_CROSS) ) // SELECT+CROSS
							{
								// SELECT+CROSS = RESERVED for custom combos

								//if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_CROSS)) && do_custom_combo("select_l2_cross")) break; // RESERVED
								//if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_CROSS)) && do_custom_combo("select_r2_cross")) break; // RESERVED
								if(do_custom_combo("select_cross")) break;    // RESERVED
							}
 #endif
						}
					} // SELECT
					else if(!(webman_config->combo & UNLOAD_WM) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_L3 | CELL_PAD_CTRL_R3))
																&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R2)) //  R2+L3+R3 / L3+R3+R2 / L3+R2+R3 (Quit / Unload webMAN)
					{
						// R2+L3+R3 / L3+R3+R2 / L3+R2+R3 = Quit / Unload webMAN
 #ifdef VIDEO_REC
						#ifdef COBRA_ONLY
						quit_plugin:
						#endif
 #endif
						wm_unload_combo = 1;

						restore_settings();

						finalize_module();
						_sys_ppu_thread_exit(0);
						break;
					}
					else if(!(webman_config->combo & GOTO_HOME) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_L3 | CELL_PAD_CTRL_R3))
																&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_L2)) //  L2+L3+R3 / L3+R3+L2 / L3+L2+R3
					{
						// L3+R3+L2    = Go to webMAN Games
						// L3+R3+L2+R2 = Go to webMAN Games + Reload Game column
						bool reload_game = (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2) == CELL_PAD_CTRL_R2;
  #ifdef WM_CUSTOM_COMBO
						if(do_custom_combo(reload_game ? "l3_r3_l2_r2" : "l3_r3_l2")) break;
						else
  #endif
						goto_xmb_home(reload_game);
						n = 0;
						break;
					}
					else
					if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_L3) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2))
					{
						// L3+R2+X     = Shutdown
						// L3+R2+O     = Lpar restart

						if(!(webman_config->combo & SHUT_DOWN) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_CROSS))) // L3+R2+X (shutdown)
						{
							// L3+R2+X = power off
							working = 0;
							del_turnoff(1); // 1 beep

							vsh_shutdown();

							sys_ppu_thread_exit(0);
						}
						else if(!(webman_config->combo & RESTARTPS) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_CIRCLE))) // L3+R2+O (lpar restart)
						{
							// L3+R2+O = lpar restart
							working = 0;
							del_turnoff(2); // 2 beeps

#ifdef COBRA_ONLY
							if(is_mamba)
								vsh_reboot();
							else
#endif
								{system_call_3(SC_SYS_POWER, SYS_REBOOT, NULL, 0);}

							sys_ppu_thread_exit(0);
						}
					}
					else if(!(webman_config->combo & DISABLEFC) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_L3 | CELL_PAD_CTRL_START)) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_R2 )) // L3+R2+START (enable/disable fancontrol)
					{
						// L3+R2+START = Enable/disable fancontrol
						enable_fan_control(TOGGLE_MODE);

						n = 0;
						break;
					}
					else if(!(webman_config->combo & DISABLEFC) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == (CELL_PAD_CTRL_L3 | CELL_PAD_CTRL_START)) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_L2 )) // L3+L2+START (enable auto #2)
					{
						// L3+L2+START = Enable Auto #2
						enable_fan_control(ENABLE_AUTO2);

						n = 0;
						break;
					}
					else
					if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_L3) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R1))
					{
						// L3+R1+X = vsh shutdown
						// L3+R1+O = vsh restart

						if(!(webman_config->combo & SHUT_DOWN) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R1 | CELL_PAD_CTRL_CROSS))) // L3+R1+X (vsh shutdown)
						{
							// L3+R1+X = vsh shutdown
							working = 0;
							del_turnoff(1); // 1 beep

							vsh_shutdown();

							sys_ppu_thread_exit(0);
						}

						if(!(webman_config->combo & RESTARTPS) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R1 | CELL_PAD_CTRL_CIRCLE))) // L3+R1+O (vsh restart)
						{
							// L3+R1+O = vsh reboot
							working = 0;
							del_turnoff(2); // 2 beeps

							vsh_reboot();

							sys_ppu_thread_exit(0);
						}
					}
					else
					if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2)
					{
						// R2+L2+TRIANGLE = Toggle user/admin mode
						// R2+L2+O + [L1/R1/L1+R1]
						// R2+TRIANGLE = disable_cfw_syscalls
						// R2+SQUARE   = block_online_servers
						// R2+CIRCLE   = show_idps / abort copy/gamefix / enable_ingame_screenshot

 #ifdef SYS_ADMIN_MODE
						if((webman_config->combo & SYS_ADMIN) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_TRIANGLE) )) // L2+R2+TRIANGLE
						{
							// R2+L2+TRIANGLE = Toggle user/admin mode
  #ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("l2_r2_triangle")) break;
							else
  #endif
							{
								sys_admin ^= 1, pwd_tries = 0;

								sprintf(msg, "ADMIN %s", sys_admin ? STR_ENABLED : STR_DISABLED);
								show_msg(msg);

								if(sys_admin) { BEEP1 } else { BEEP2 }
							}
							n = 0;
							break;
						}
						else
 #endif
						if(!(webman_config->combo & SHOW_IDPS) && ( (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2 | CELL_PAD_CIRCLE_BTN)) == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2 | CELL_PAD_CIRCLE_BTN) ) && IS_ON_XMB) // L2+R2+O
						{
							// L2+R2+O + [L1/R1/L1+R1] = Open Browser file manager / cpursx / games / setup
							// L2+R2+X + [L1/R1/L1+R1] = Open Browser file manager / cpursx / games / setup (JAP)
#ifdef WM_CUSTOM_COMBO
								 if(do_custom_combo("l2_r2_circle")) ;
							else if(do_custom_combo("l2_r2_l1_circle")) ;
							else if(do_custom_combo("l2_r2_r1_circle")) ;
							else if(do_custom_combo("l2_r2_l1_r1_circle")) ;
							else
#endif
							{
#ifdef PS3_BROWSER
								do_umount(false); // prevent system freeze on disc icon

								if( pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L1 | CELL_PAD_CTRL_R1 | CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_R2 | CELL_PAD_CIRCLE_BTN) )
									{open_browser((char*)"http://127.0.0.1/setup.ps3", 0); show_msg(STR_WMSETUP);}     // L2+R2+L1+R1+O  ||  L2+R2+L1+R1+X (JAP)
								else if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R1)
									{open_browser((char*)"http://127.0.0.1/index.ps3", 0); show_msg(STR_MYGAMES);}     // L2+R2+R1+O  ||  L2+R2+R1+X (JAP)
								else if(pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_L1)
									{open_browser((char*)"http://127.0.0.1/cpursx.ps3", 0); show_msg(WM_APPNAME " Info");}  // L2+R2+L1+O || L2+R2+L1+X (JAP)
								else
									{open_browser((char*)"http://127.0.0.1/", 0); show_msg(WM_APP_VERSION);}           // L2+R2+O || L2+R2+X (JAP)
#endif
							}
						}
						else
						if((copy_in_progress || fix_in_progress) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CIRCLE_BTN)) )  // R2+O Abort copy process
						{
							fix_aborted = copy_aborted = true;
						}
						else
						if(!( webman_config->combo & DISABLESH ) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_TRIANGLE))) // R2+TRIANGLE Disable CFW Sycalls
						{
#ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("r2_triangle")) break;
							else
#endif
							{
#ifdef REMOVE_SYSCALLS
								disable_cfw_syscalls(webman_config->keep_ccapi);
#endif
							}
						}
						else
						if(!(webman_config->combo2 & CUSTOMCMB) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CTRL_SQUARE))) // R2+SQUARE
						{
#ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("r2_square")) break;
							else
#endif
#ifdef WM_REQUEST
							if(do_custom_combo(WM_COMBO_PATH)) break;
							else
#endif
							{
								block_online_servers(true);
							}
						}
						else
						if(!(webman_config->combo & SHOW_IDPS) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_R2 | CELL_PAD_CIRCLE_BTN)) )  // R2+O Show IDPS EID0+LV2 (JAP)
						{
#ifdef WM_CUSTOM_COMBO
							if(do_custom_combo("r2_circle")) break;
							else
#endif
							{
								char path[STD_PATH_LEN];
								char *sfo = (char*)"/dev_bdvd/PS3_GAME/PARAM.SFO";
								if(!file_exists(sfo)) sfo = (char*)"/app_home/PARAM.SFO";

								if(file_exists(sfo) || IS_INGAME)
								{
									char title[128], title_id[12], version[8], version2[8];

									if(get_game_info())
									{
										strcpy(title, _game_Title);
										strcpy(title_id, _game_TitleID);
									}
									else
									{
										strcpy(title, sfo);
										getTitleID(title, title_id, GET_TITLE_AND_ID);
									}

									int len = sprintf(msg, "ID: %s", title_id);

									strcpy(path, sfo);
									getTitleID(path, version, GET_VERSION);

									sprintf(path, "%s%s/PARAM.SFO", HDD0_GAME_DIR, title_id);
									if(!file_exists(path))
										strcpy(path, sfo);

									getTitleID(path, version2, GET_VERSION); if(*version2) strcpy(version, version2);
									get_last_game(path);

									sprintf(msg + len, " - v%s\n%s\n\n%s", version, title, path);
									show_msg(msg);
									sys_ppu_thread_sleep(5);
								}
								else if(file_exists("/dev_bdvd/SYSTEM.CNF"))
								{
									read_file("/dev_bdvd/SYSTEM.CNF", (void*)msg, 200, 0);

									get_last_game(path);

									strcat(msg, "\n"); strcat(msg, path);
									show_msg(msg);
									sys_ppu_thread_sleep(5);
								}

								#ifdef SPOOF_CONSOLEID
								show_idps(msg);
								#endif

								// backup / restore act.bak -> act.dat
								backup_act_dat();
							}
						}

						n = 0;
						break;
					}
					else
					if((pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] == CELL_PAD_CTRL_L3) && (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_L2))
					{
						// L3+L2+TRIANGLE = COBRA Toggle
						// L3+L2+SQUARE   = REBUG Mode Switcher
						// L3+L2+CIRCLE   = Normal Mode Switcher
						// L3+L2+X        = DEBUG Menu Switcher

						if(!sys_admin || IS_INGAME) continue;
#ifdef COBRA_ONLY
 #ifndef LITE_EDITION
						if(!(webman_config->combo & DISACOBRA)
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_TRIANGLE)))
						{ // L3+L2+TRIANGLE COBRA Toggle
							reboot = toggle_cobra();
						}
 #endif
#endif //#ifdef COBRA_ONLY

#ifdef REX_ONLY
						if(!(webman_config->combo2 & REBUGMODE)
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_SQUARE)))
						{ // L3+L2+SQUARE REBUG Mode Switcher
							reboot = toggle_rebug_mode();
						}
						else
						if(!(webman_config->combo2 & NORMAMODE)
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_CIRCLE)))
						{ // L3+L2+O Normal Mode Switcher
							reboot = toggle_normal_mode();
						}
						else
						if(!(webman_config->combo2 & DEBUGMENU)
							&& (pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == (CELL_PAD_CTRL_L2 | CELL_PAD_CTRL_CROSS)))
						{ // L3+L2+X DEBUG Menu Switcher
							toggle_debug_menu();
						}
#endif //#ifdef REX_ONLY
					}
				}

				if(reboot)
				{
					sys_ppu_thread_sleep(1);
					// reboot
					show_msg("Switching successful! Reboot now...");
					sys_ppu_thread_sleep(3);
					disable_dev_blind();
reboot:
					// vsh reboot
					working = 0;

					del_turnoff(0); // no beep
					save_file(WMNOSCAN, NULL, SAVE_ALL);

					vsh_reboot(); // VSH reboot

					sys_ppu_thread_exit(0);
				}
			}

			sys_ppu_thread_usleep(300000);

			if(show_persistent_popup)
			{
				show_persistent_popup++;
				if(show_persistent_popup > 10 && show_persistent_popup < PERSIST) show_persistent_popup = 0; else
				if(show_persistent_popup > PERSIST + 6) show_persistent_popup = PERSIST;
			}
		}

		if(working && (n < 10)) sys_ppu_thread_usleep((12 - n) * 150000);
