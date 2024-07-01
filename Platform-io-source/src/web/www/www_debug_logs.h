#pragma once

const char debug_logs_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html data-bs-theme="%THEME%">
	<head>
		<title>TinyWATCH S3 | Debug Logs</title>
		%META%
	</head>
	<body>
		<div class="content" style="margin-top:12px;">
			<h2 class="center">TinyWATCH S3 | Debug Logs</h2>

			<div class="content" style="margin:30px;">
				<div class="row">
					<div class='col-3'>&nbsp;</div>
					<div class='col-2 center'><a href='/index.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>WATCH</button></a></div>
					<div class='col-2 center'><a href='/web_settings_widgets.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>WIDGETS</button></a></div>
					<div class='col-2 center'><a href='/web_settings_apps.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>APPS</button></a></div>
					<div class='col-3'>&nbsp;</div>
				</div>
			</div>

			<div class="content ps-3 pe-3 text-center">
				<h3>Debug Logs</h3>
				%DEBUG_LOGS%
			</div>

		%FOOTER%

)rawliteral";
