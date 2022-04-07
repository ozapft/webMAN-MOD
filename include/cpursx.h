#define SYS_NET_EURUS_POST_COMMAND		(726)
#define CMD_GET_MAC_ADDRESS				0x103f

#define FAHRENHEIT(celsius)	((int)(1.8f * (float)celsius + 32.f))

/*
static u32 in_cobra(u32 *mode)
{
	system_call_2(SC_COBRA_SYSCALL8, (u32) 0x7000, (u32)mode);
	return_to_user_prog(u32);
}
*/

static s32 sys_sm_request_be_count(s32 *arg_1, s32 *total_time_in_sec, s32 *power_on_ctr, s32 *power_off_ctr)
{
	system_call_4(0x187, (u32)arg_1, (u32)total_time_in_sec, (u32)power_on_ctr, (u32)power_off_ctr);
	return_to_user_prog(s32);
}

static void sys_get_cobra_version(void)
{
#ifdef COBRA_ONLY
	if(payload_ps3hen)
		{system_call_1(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_HEN_REV); cobra_version = (int)p1;}
	else
	{
		if(!is_mamba) {system_call_1(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_GET_MAMBA); is_mamba = ((int)p1 == 0x666);}
		sys_get_version2(&cobra_version);
	}
#endif
}

static void get_cobra_version(char *cfw_info)
{
	// returns cfw_info[22]

#ifdef COBRA_ONLY
	syscalls_removed = CFW_SYSCALLS_REMOVED(TOC);
	if(!syscalls_removed) disable_signin_dialog();

	if(!cobra_version && !syscalls_removed) sys_get_cobra_version();

	char cobra_ver[12];
	if((cobra_version & 0x0F) == 0)
		sprintf(cobra_ver, "%X.%X", cobra_version>>8, (cobra_version & 0xFF) >> 4);
	else
		sprintf(cobra_ver, "%X.%02X", cobra_version>>8, (cobra_version & 0xFF));

	if(payload_ps3hen) {sprintf(cfw_info, "%s %s %s", dex_mode ? "DEX" : "CEX", "PS3HEN", cobra_ver); return;}

	#if defined(DECR_SUPPORT)
		sprintf(cfw_info, "%s %s %s", (dex_mode == 1) ? "DECR" : dex_mode ? "DEX" : "CEX", is_mamba ? "Mamba" : "Cobra", cobra_ver);
	#elif defined(DEX_SUPPORT)
		sprintf(cfw_info, "%s %s %s", dex_mode ? "DEX" : "CEX", is_mamba ? "Mamba" : "Cobra", cobra_ver);
	#else
		sprintf(cfw_info, "%s %s %s", "CEX", is_mamba ? "Mamba" : "Cobra", cobra_ver);
	#endif
	if(!cobra_version) {char *cfw = strchr(cfw_info, ' '); *cfw = NULL;}
#elif defined(DEX_SUPPORT)
	#if defined(DECR_SUPPORT)
		sprintf(cfw_info, "%s", (dex_mode == 1) ? "DECR" : dex_mode ? "DEX" : "CEX");
	#else
		sprintf(cfw_info, "%s", dex_mode ? "DEX" : "CEX");
	#endif
#else
		sprintf(cfw_info, "CEX");
#endif
#ifndef COBRA_ONLY
		sprintf(cfw_info, " nonCobra");
#endif

	// noBD LV1 4.75 - 4.88
	if(isNOBD)  // ori: 0x78630020409E0018ULL
		strcat(cfw_info, " noBD");
}

static void get_net_info(char *net_type, char *ip)
{
	// returns net_type[8], ip[ip_size]

	s32 status = 0; xnet()->GetSettingNet_enable(&status);

	if(status == 0) {strcpy(net_type, "OFFLINE"); *ip = NULL; return;}

	net_info info;
	memset(&info, 0, sizeof(net_info));
	xnet()->sub_44A47C(&info); //info.ipAddress

	if (info.device == 0) strcpy(net_type, "LAN"); else
	if (info.device == 1) strcpy(net_type, "WLAN");

	netctl_main_9A528B81(ip_size, ip);
}

static void add_game_info(char *buffer, char *templn, u8 is_cpursx)
{
	if(IS_INGAME)
	{
		if(!is_cpursx && sys_admin)
		{
			if(GetCurrentRunningMode() == 1)
			{
#ifdef GET_KLICENSEE
				buffer += concat(buffer, " [<a href=\"/klic.ps3\">KLIC</a>]");
#endif
#ifdef SYS_BGM
				buffer += concat(buffer, " [<a href=\"/sysbgm.ps3\">BGM</a>]");
#endif
#ifdef VIDEO_REC
				buffer += concat(buffer, " [<a href=\"/videorec.ps3\">REC</a>]");
#endif
				buffer += concat(buffer, " [<a href=\"/xmb.ps3$reloadgame\">Reload</a>]");
			}
			buffer += concat(buffer, " [<a href=\"/xmb.ps3$exit\">Exit</a>]");
		}

		get_game_info();

		if(strlen(_game_TitleID) == 9)
		{
			sprintf(templn, "<hr><span style=\"position:relative;top:-20px;\"><H2><a href=\"%s/%s/%s-ver.xml\" target=\"_blank\">%s</a>", "https://a0.ww.np.dl.playstation.net/tpl/np", _game_TitleID, _game_TitleID, _game_TitleID); buffer += concat(buffer, templn);

			char *title = templn + 0x10, *title_id = templn;
			char path[MAX_PATH_LEN], app_ver[8];

			set_param_sfo(path);
			get_game_version(path, title, title_id, app_ver);
			remove_filename(path);

			sprintf(templn, " <a href=\"%s%s\">%s %s</a> &nbsp; ", search_url, _game_Title, _game_Title, app_ver); buffer += concat(buffer, templn);

			sprintf(templn, "<a href=\"%s\"><img src=\"%s/ICON0.PNG\" height=\"60\" border=0%s></a> "
							"<a href=\"/%s.ps3mapi?proc=%i\"><small>pid=%08x</small></a>",
					path, path, " style=\"position:relative;top:20px;\"", (is_cpursx < 3) ? "gameplugin" : "getmem",  GetGameProcessID(), GetGameProcessID()); buffer += concat(buffer, templn);

			buffer += concat(buffer, "</H2></span>");
		}
	}
}

static void cpu_rsx_stats(char *buffer, char *templn, char *param, u8 is_ps3_http)
{
	{ PS3MAPI_ENABLE_ACCESS_SYSCALL8 }

	u8 t1 = 0, t2 = 0, t1f, t2f;
	get_temperature(0, &t1); // CPU // 3E030000 -> 3E.03°C -> 62.(03/256)°C
	get_temperature(1, &t2); // RSX

	t1f = FAHRENHEIT(t1);
	t2f = FAHRENHEIT(t2);

	_meminfo meminfo;
	{system_call_1(SC_GET_FREE_MEM, (u64)(u32) &meminfo);}

	if((webman_config->fanc == DISABLED) && (get_fan_policy_offset > 0))
	{
		u8 st, mode, unknown;
		sys_sm_get_fan_policy(0, &st, &mode, &fan_speed, &unknown);

		if(strstr(param, "?u") || strstr(param, "?d")) enable_fan_control(ENABLE_SC8);
	}

#ifdef SPOOF_CONSOLEID
	get_eid0_idps();
#endif

	if(sys_admin && !webman_config->sman && !strstr(param, "/sman.ps3")) {sprintf(templn, " [<a href=\"/shutdown.ps3\">%s</a>] [<a href=\"/restart.ps3\">%s</a>]", STR_SHUTDOWN, STR_RESTART ); buffer += concat(buffer, templn);}

	add_game_info(buffer, templn, 0); *templn = 0;

#ifdef COPY_PS3
	if(ftp_state)
	{
		sprintf(templn, "<hr><font size=2>FTP: %s %s</font>", (ftp_state == 1) ? "Sending " : "Receiving ", current_file);
	}
	else
	if(copy_in_progress)
	{
		sprintf(templn, "<hr><font size=2><a href=\"%s$abort\">&#9746 %s</a> %s (%i %s)</font>", "/copy.ps3", STR_COPYING, current_file, copied_count + 1, STR_FILES);
	}
	else if(fix_in_progress)
	{
		sprintf(templn, "<hr><font size=2><a href=\"%s$abort\">&#9746 %s</a> %s (%i %s)</font>", "/fixgame.ps3", STR_FIXING, current_file, fixed_count, STR_FILES);
	}
	else
#endif
	if(IS_ON_XMB && ((View_Find("game_plugin") != 0) || (View_Find("download_plugin") != 0)) )
	{
		sprintf(templn, "<hr><font size=2>&starf; Status: %s %s</font>",
						View_Find("download_plugin") ? "Downloading file" : "",
						View_Find("game_plugin")     ? "Installing PKG"   : "");
	}
	/*else
	{
		unsigned int real_disctype, effective_disctype, iso_disctype;
		cobra_get_disc_type(&real_disctype, &effective_disctype, &iso_disctype);
		sprintf(templn, "<font size=2>Disc Type: real=%i, effective=%i, iso=%i</font><br>", real_disctype, effective_disctype, iso_disctype);
	}*/

	if(*templn) buffer += concat(buffer, templn);

	if(strchr(param, '?'))
	{
		char *pos = strstr(param, "fan=");  // 0 = SYSCON, 1 = DYNAMIC, 2 = FAN_AUTO2
		if(pos)
		{
			u32 new_speed = get_valuen(param, "fan=", 0, webman_config->maxfan); max_temp = 0;
			if(new_speed <= ENABLED)
			{
				webman_config->fanc = new_speed;
				enable_fan_control(new_speed);
			}
			else if(new_speed == FAN_AUTO2)
			{
				webman_config->fanc = FAN_AUTO2;
				enable_fan_control(ENABLE_AUTO2);
			}
			else
			{
				max_temp = FAN_MANUAL;
				webman_config->man_rate = RANGE(new_speed, webman_config->minfan, webman_config->maxfan);
				webman_config->man_speed = (u8)(((float)(webman_config->man_rate + 1) * 255.f)/100.f); // manual fan speed
				if(webman_config->fanc == DISABLED) enable_fan_control(ENABLE_SC8);
				set_fan_speed(webman_config->man_speed);
			}
		}
		else if(webman_config->fanc || strstr(param, "?m"))
		{
			pos = strstr(param, "max=");
			if(pos)
				max_temp = get_valuen(param, "max=", 40, MAX_TEMPERATURE);
			else
			{
				pos = strstr(param, "?m");
				if(pos)
				{
					if(webman_config->fanc == FAN_AUTO2) enable_fan_control(ENABLED);

					if((max_temp && !strstr(param, "dyn")) || strstr(param, "man"))
						max_temp = FAN_MANUAL;
					else
						max_temp = webman_config->dyn_temp;

					if(webman_config->fanc == DISABLED) enable_fan_control(ENABLE_SC8);
				}
			}

			if(strstr(param, "?mode=s"))
				enable_fan_control(DISABLED);
			else if(strstr(param, "?mode=a"))
				enable_fan_control(ENABLE_AUTO2);
			else if(max_temp) //auto mode
			{
				if(strstr(param, "?u")) max_temp++;
				if(strstr(param, "?d")) max_temp--;
				webman_config->dyn_temp = RANGE(max_temp, 40, MAX_TEMPERATURE); // dynamic fan max temperature in °C
				webman_config->man_speed = FAN_AUTO;

				fan_ps2_mode = false;
			}
			else
			{
				if(strstr(param, "?u")) webman_config->man_rate++;
				if(strstr(param, "?d")) webman_config->man_rate--;
				webman_config->man_rate = RANGE(webman_config->man_rate, 20, 95); //%

				reset_fan_mode();
			}
		}

		save_settings();
	}

	{ PS3MAPI_ENABLE_ACCESS_SYSCALL8 }

	char max_temp1[128], max_temp2[64]; *max_temp2 = NULL;

	if(fan_ps2_mode || ps2_classic_mounted)
	{
		sprintf(max_temp1, " (PS2 Mode: %i%%)", webman_config->ps2_rate);
	}
	else if((webman_config->fanc == DISABLED) || (!webman_config->man_speed && !max_temp))
		sprintf(max_temp1, "<small>[%s %s]</small>", STR_FANCTRL3, "SYSCON");
	else if(webman_config->fanc == FAN_AUTO2)
	{
		sprintf(max_temp1, "(AUTO)");
		sprintf(max_temp2, "(AUTO)");
	}
	else if(max_temp)
	{
		sprintf(max_temp1, "(MAX: %i°C)", max_temp);
		sprintf(max_temp2, "(MAX: %i°F)", FAHRENHEIT(max_temp));
	}
	else
		sprintf(max_temp1, "<small>[FAN: %i%% %s]</small>", webman_config->man_rate, STR_MANUAL);

	*templn = NULL;

	char hdd_free[40];

#ifndef LITE_EDITION
	for(u8 d = 1; d < 7; d++)
	{
		if(isDir(drives[d]))
		{
			free_size(drives[d], hdd_free);
			sprintf(param, "<br><a href=\"%s\">USB%.3s: %s</a>", drives[d], drives[d] + 8, hdd_free); strcat(templn, param);
		}
	}
#endif

	sprintf(param,	"<hr><font size=\"42px\">"
					"<b><a class=\"s\" href=\"/cpursx.ps3?up\">"
					"CPU: %i°C %s<br>"
					"RSX: %i°C</a><hr>"
					"<a class=\"s\" href=\"/cpursx.ps3?dn\">"
					"CPU: %i°F %s<br>"
					"RSX: %i°F</a><hr>",
					t1, max_temp1, t2,
					t1f, max_temp2, t2f); buffer += concat(buffer, param);

	if(IS_ON_XMB && file_exists(WM_RES_PATH "/slaunch.sprx"))
		sprintf(max_temp1, "/browser.ps3$slaunch");
	else
		sprintf(max_temp1, "/games.ps3");

	free_size(drives[0], hdd_free);

	sprintf(param,	"<a class=\"s\" href=\"%s\">"
					"MEM: %'d KB %s</a><br>"
					"<a href=\"%s\">HDD: %s</a>%s<hr>"
					"<a class=\"s\" href=\"/cpursx.ps3?mode\">"
					"%s %i%% (0x%X)</a><br>",
					max_temp1, (meminfo.avail>>10), IS_ON_XMB ? "(XMB)" : "",
					drives[0], hdd_free, templn,
					STR_FANCH2, (int)((int)fan_speed * 100) / 255, fan_speed); buffer += concat(buffer, param);

	if(!max_temp && webman_config->fanc && !is_ps3_http )
	{
		sprintf(templn, "<input type=\"range\" value=\"%i\" min=\"%i\" max=\"%i\" style=\"width:600px\""
						" onchange=\"self.location='/cpursx.ps3?fan='+this.value\">",
						webman_config->man_rate, webman_config->minfan, webman_config->maxfan);
		buffer += concat(buffer, templn);
	}

	strcat(buffer, "<hr>");

	CellRtcTick pTick; cellRtcGetCurrentTick(&pTick); u32 dd, hh, mm, ss;

	// detect aprox. time when a game is launched
	if(IS_ON_XMB) gTick=rTick; else if(gTick.tick==rTick.tick) cellRtcGetCurrentTick(&gTick);

	////// play time //////
	if(gTick.tick > rTick.tick)
	{
		ss = (u32)((pTick.tick - gTick.tick) / 1000000);
		dd = (u32)(ss / 86400); ss %= 86400; hh = (u32)(ss / 3600); ss %= 3600; mm = (u32)(ss / 60); ss %= 60;
		if(dd<100) {sprintf( templn, "<label title=\"Play\">&#9737;</label> %id %02d:%02d:%02d<br>", dd, hh, mm, ss); buffer += concat(buffer, templn);}
	}
	///////////////////////

	// get runtime data by @3141card
	int32_t arg_1, total_time_in_sec, power_on_ctr, power_off_ctr;
	sys_sm_request_be_count(&arg_1, &total_time_in_sec, &power_on_ctr, &power_off_ctr);

	//// startup time /////
	ss = (u32)((pTick.tick - rTick.tick) / 1000000); total_time_in_sec += ss;
	dd = (u32)(ss / 86400); ss %= 86400; hh = (u32)(ss / 3600); ss %= 3600; mm = (u32)(ss / 60); ss %= 60;

	if(webman_config->chart)
		sprintf(templn, "<a href=\"%s\">", CPU_RSX_CHART);
	else
		sprintf(templn, "<a href=\"%s/%08i\">", HDD0_HOME_DIR, xusers()->GetCurrentUserNumber()); buffer += concat(buffer, templn);

	sprintf(templn, "<label title=\"Startup\"><img src='%s/time.png' border=0 style='position:relative;top:8px;'></label> %id %02d:%02d:%02d</a>", WM_ICONS_PATH, dd, hh, mm, ss); buffer += concat(buffer, templn);
	///////////////////////

	ss = (u32)total_time_in_sec;
	dd = (u32)(ss / 86400); ss %= 86400; hh = (u32)(ss / 3600); ss %= 3600; mm = (u32)(ss / 60); ss %= 60;

	sprintf(templn, "</font><H1><img src='%s/power.png' style='position:relative;top:8px;'> %'id %02d:%02d:%02d • %'i ON • %'i OFF (%i)</H1>", WM_ICONS_PATH, dd, hh, mm, ss, power_on_ctr, power_off_ctr, power_on_ctr - power_off_ctr); buffer += concat(buffer, templn);

	if(isDir("/dev_bdvd"))
	{
		get_last_game(param);

		if(*param == '/') {sprintf( templn, "<hr><font size=\"3\">" HTML_URL " -> ", IS_ON_XMB ? "/play.ps3" : "/dev_bdvd", "/dev_bdvd"); buffer += concat(buffer, templn); add_breadcrumb_trail(buffer, param); buffer += concat(buffer, "</font>");}
	}

	// Get mac address [0xD-0x12]
	if(sys_admin)
	{
		u8 mac_address[0x13];
		{system_call_3(SYS_NET_EURUS_POST_COMMAND, CMD_GET_MAC_ADDRESS, (u64)(u32)mac_address, 0x13);}

		char *cfw_info = param;
		get_cobra_version(cfw_info);

		char net_type[8] = "", ip[ip_size] = "-";
		get_net_info(net_type, ip);

		sprintf(templn, "<hr><h2>"
						"<input type=button onclick=\"document.getElementById('ht').style.display='block';\" value='&#x25BC;'> "
						"<a class=\"s\" href=\"/setup.ps3\">"
						"%s : %s %s<br>"
						"%s<br>"
						"<span id='ht' style='display:none;'>"
#ifdef SPOOF_CONSOLEID
						"PSID LV2 : %016llX%016llX<hr>"
						"IDPS EID0: %016llX%016llX<br>"
						"IDPS LV2 : %016llX%016llX<br>"
#endif
						"MAC Addr : %02X:%02X:%02X:%02X:%02X:%02X - %s %s"
						"</span></h2></a></b>",
						STR_FIRMWARE, fw_version, cfw_info,
						(syscalls_removed) ? STR_CFWSYSALRD : "",
#ifdef SPOOF_CONSOLEID
						PSID[0], PSID[1],
						eid0_idps[0], eid0_idps[1],
						IDPS[0], IDPS[1],
#endif
						mac_address[13], mac_address[14], mac_address[15], mac_address[16], mac_address[17], mac_address[18], ip, net_type); buffer += concat(buffer, templn);
	}

	/////////////////////////////
#ifdef COPY_PS3
	if(copy_in_progress)
	{
		sprintf( templn, "<hr>%s %s (%i %s)", STR_COPYING, current_file, copied_count, STR_FILES); buffer += concat(buffer, templn);
	}
	else
	if(fix_in_progress)
	{
		sprintf( templn, "<hr>%s %s (%i %s)", STR_FIXING, current_file, fixed_count, STR_FILES); buffer += concat(buffer, templn);
	}
#endif
	/////////////////////////////

	buffer += concat(buffer, HTML_BLU_SEPARATOR
							 WM_APP_VERSION " - Simple Web Server" EDITION " ");

	if(webman_config->combo & SYS_ADMIN) strcat(buffer, sys_admin ? "[ADMIN]":"[USER]");

	strcat(buffer, "<p>");

	{ PS3MAPI_DISABLE_ACCESS_SYSCALL8 }
}

static void get_cpursx(char *cpursx)
{
	u8 t1 = 0, t2 = 0;
	get_temperature(0, &t1); // CPU // 3E030000 -> 3E.03°C -> 62.(03/256)°C
	get_temperature(1, &t2); // RSX

	sprintf(cpursx, "CPU: %i°C | RSX: %i°C", t1, t2);
}

static void show_wm_version(char *param)
{
	char cfw_info[24];
	get_cobra_version(cfw_info);

	if(payload_ps3hen)
	{
		sprintf(param,	"%s\n"
						"%s %s", WM_APP_VERSION, cfw_info + 4, STR_ENABLED);
	}
	else
	{
		if(!cobra_version) sprintf(cfw_info, "[nonCobra]");
		sprintf(param,	"%s\n"
						"%s %s" EDITION, WM_APP_VERSION, fw_version, cfw_info);
	}
	show_msg(param);
}
