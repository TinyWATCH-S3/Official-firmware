#pragma once

const char index_settings_widgets_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html data-bs-theme="%THEME%">
	<head>
		<title>TinyWATCH S3 | Web Settings | Widgets</title>
		%META%
	</head>
	<body>
		<div class="content" style="margin-top:12px;">
			<h2 class="center">TinyWATCH S3 | Web Portal | Widgets</h2>

			<div class="content" style="margin:30px;">
				<div class="row">
					<div class='col-1'>&nbsp;</div>
					<div class='col-2 center'><a href='/index.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>WATCH</button></a></div>
					<div class='col-2 center'><a href='/web_settings_web.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>WIFI&nbsp;&&nbsp;WEB</button></a></div>
					<div class='col-2 center'><button class='btn btn-sm btn-primary' style='width:130px;'>WIDGETS</button></div>
					<div class='col-2 center'><a href='/web_settings_apps.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>APPS</button></a></div>
					<div class='col-2 center'><a href='/web_settings_themes.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>THEMES</button></a></div>
					<div class='col-1'>&nbsp;</div>
				</div>
			</div>

			<div class="content">

				%SETTING_OPTIONS_WIDGETS%

			</div>

		%FOOTER%

		<script>
			document.addEventListener('htmx:afterSwap', function(evt) {
				// Ensure the target container is one of the form containers
				if (evt.detail.target.id.startsWith("settings_group_")) {
					var flashSpan = evt.detail.target.querySelector('.flash-span');
					if (flashSpan) {
						flashSpan.style.display = 'inline';
						flashSpan.classList.add('flash_post');
						setTimeout(function() {
							flashSpan.classList.remove('flash_post');
							flashSpan.style.display = 'none';
						}, 1000); // Adjust the duration as needed
					}
				}
			});
		</script>

)rawliteral";
