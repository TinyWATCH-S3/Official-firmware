#pragma once

// html strings to be used only from wifi_setup.cpp

static const char *startHtml = R"~(
<!DOCTYPE html>
<html>
<title>TinyWATCH S3 | Wifi Setup</title>
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<style>
		html { background-color: #333; } 
		body {
			min-height: 90vh;
			max-width: 90vw;
			background-color: #fff; 
			margin: auto;
			padding:10px;
			font-size: 9pt;
			font-family: Helvetica, Arial, sans-serif;
		}

		h2 {
		font-size: 16pt;
		}

		h3 {
		font-size: 14pt;
		}

		h4 {
		font-size: 12pt;
		}

		.ssid {
			font-size: 14pt;
			line-height:20pt;
		}

		.ssid_extra {
			font-size: 14pt;
			line-height:20pt;
		}


		.error {
			color: rgba(202, 60, 60, 0.2);
		}

		.clear {
		background: #fff;
		}

		.alt {
		background: #eee;
		}

		.heading {
		background-color:#005cb5;
		color:#fff;
		padding:3px;
		}

		.right {
		text-align:right;
		}

		.center {
		text-align:center;
		}

		.footer {
			text-align:center;
			color:#999;
			font-size:9pt;
			margin-top:12px;
		}

		.cell_overflow {
		overflow-x: hidden;
		text-overflow: ellipsis;
		width: 100%%;
		white-space: nowrap;
		}

		.content {
		max-width: 900px;
		margin: auto;
		}

		.no-wrap .nowrap {
			display:inline-block;
			white-space: nowrap;
		}

		.button {
			background-color: #007CFF;
			border-radius: 6px;
			border-style: none;
			box-sizing: border-box;
			color: #FFFFFF;
			cursor: pointer;
			display: inline-block;
			font-size: 13px;
			font-weight: 500;
			height: 32px;
			line-height: 20px;
			list-style: none;
			margin: 0;
			outline: none;
			padding: 8px 12px;
			position: relative;
			text-align: center;
			text-decoration: none;
			transition: color 100ms;
			vertical-align: baseline;
			user-select: none;
			-webkit-user-select: none;
			touch-action: manipulation;
		}

		.button:hover,
		.button:focus {
			background-color: #00C5FF;
		}

		.green {
			background-color: #00C559;
		}

		.green_text {
			color: #00C559;
		}

		input {
			font-size: 16px;
			font-size: max(16px, 1em);
			font-family: inherit;
			padding: 0.25em 0.5em;
			background-color: #fff;
			border: 2px solid #666;
			border-radius: 4px;
			margin: 1px;
		}
	</style>
</head>
<body>
	<h1>TinyWATCH S3 Wifi Setup</h1>
	<div class="content">
)~";

static const char *scanningHtml = R"~(
<meta http-equiv="refresh" content="0;url=/connect">
<h3 class="green_text">Scanning WiFi Networks...</h3>
)~";

static const char *formHtml = R"~(

	<div class="content">
		<h3>Connect to WiFi Network</h3>
		<form method="post" style="font-size:14pt;">
			<input name="ssid" required placeholder="SSID">
			<input name="pass" type="password" placeholder="PASSWORD">
			<div style="margin-top:6px;"><a href="/"><button class="button green" role="button" type="button" style="margin-top;3px;">RE-SCAN</button></a>&nbsp;<button class="button" role="button"><strong>CONNECT</strong></button></div>
		</form>
	</div>

<script>
document.querySelectorAll(".ssid").forEach(link => {
	link.addEventListener("click", event => {
		event.preventDefault();
		document.querySelector("[name=ssid]").value = event.target.textContent;
		document.querySelector("[name=pass]").focus();
	});
});
</script>
</html>
)~";

static const char *connectingHtml = R"~(
<meta http-equiv="refresh" content="0;url=/connectresult">
<h3 class="green_text">Connecting to {ssid}...</h3>
)~";

static const char *footerHtml = R"~(
		<div class="footer">TinyWATCH S3 - 2023 Unexpected Maker</div>
	</body>
</html>
)~";

static const char *retryHtml = R"~(
		<h2 class="error">Failed to connect!</h2>
		<div class="content"><a href="/"><button class="button green" role="button" style="margin-top;3px;">RETRY</button></a></div>
)~";
