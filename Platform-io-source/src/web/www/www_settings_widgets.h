#pragma once

// html strings to be used only from wifi_setup.cpp
const char index_settings_widgets_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html data-bs-theme="%THEME%">
	<head>
		<title>TinyWATCH S3 | Web Settings | Widgets</title>
		%META%
	</head>
	<body>
		<div class="content" style="margin-top:12px;">
			<h2 class="center">TinyWATCH S3 | Web Settings | Widgets</h2>

			<div class="content" style="margin:30px;">
				<div class="row">
					<div class='col-3'>&nbsp;</div>
					<div class='col-2 center p-2'><a href='/index.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>WATCH</button></a></div>
					<div class='col-2 center p-2'><button class='btn btn-sm btn-primary' style='width:130px;'>WIDGETS</button></div>
					<div class='col-2 center p-2'><a href='/web_settings_apps.html'><button class='btn btn-sm btn-outline-primary' style='width:130px;'>APPS</button></a></div>
					<div class='col-3'>&nbsp;</div>
				</div>
			</div>

			<div class="content">

				<form hx-post="/update_widget_ow">

				<div class="container">

					<span class="settings_heading">Open Weather</span>
					<div class="settings_frame" style="margin-bottom:5px; padding-bottom:5px;">

						<div class="row">
							<div class="col-5">

								<div class="input-group input-group-sm mb-1 flex-nowrap">
									<span class="input-group-text" id="inputGroup-sizing-sm">Enabled</span>
									<select class="form-select form-select-sm" id="_set_widget_ow_enable" name="_set_widget_ow_enable"  onchange="set_input_states();">
										<option value="1" %SET_WID_OW_ENABLE_YES%>YES</option>
										<option value="0" %SET_WID_OW_ENABLE_NO%>NO</option>
									</select>
								</div>
								
							</div>
							<div class="col-7">

								<div class="input-group input-group-sm mb-1 flex-nowrap">
									<span class="input-group-text" id="inputGroup-sizing-sm">API KEY</span>
									<div class="reset_field_div">
										<input type="text" class="form-control form-control-sm" name="_set_widget_ow_api_key" id="_set_widget_ow_api_key" value="%SET_WID_OW_API_KEY%" required onchange="" />
									</div>
								</div>

							</div>
						</div>
						<div class="row">

							<div class="col-8">

								<div class="input-group input-group-sm mb-1 flex-nowrap">
									<span class="input-group-text" id="inputGroup-sizing-sm">Poll Frequency</span>
									<div class="reset_field_div">
										<input type="number" class="form-control form-control-sm" name="_set_widget_ow_poll_frequency" id="_set_widget_ow_poll_frequency" value="%SET_WID_OW_POLL_FREQ%" required onchange="" />
									</div>
								</div>

							</div>

							<div class="col-4">
								<button type="submit" class='btn btn-sm btn-primary''>UPDATE</button>
							</div>
								
						</div>
					</div>
				</form>

			</div>
			</div>

		%FOOTER%

)rawliteral";
