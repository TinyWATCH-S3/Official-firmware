#pragma once

// html strings to be used only from wifi_setup.cpp
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html data-bs-theme="%THEME%">
	<head>
		<title>TinyWATCH S3 | Web Settings</title>
		%META%
	</head>
	<body>
		<div class="content" style="margin-top:12px;">
			<h2 class="center">TinyWATCH S3 | Web Settings | Watch</h2>

			<div class="content" style="margin:30px;">
				<div class="row">
					<div class='col-3'>&nbsp;</div>
					<div class='col-2 center'><button class='btn btn-sm btn-primary' style='width:130px;'>WATCH</button></div>
					<div class='col-2 center'><a href='/web_settings_widgets.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>WIDGETS</button></a></div>
					<div class='col-2 center'><a href='/web_settings_apps.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>APPS</button></a></div>
					<div class='col-3'>&nbsp;</div>
				</div>
			</div>

			<div class="content" style="margin:30px;">
				<div class="row">
				</div>
			</div>

		%FOOTER%

)rawliteral";
