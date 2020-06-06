// Data handling functions
send = function(data, successFunction) {
	$.ajax({
		type: "POST", url: "/", data: JSON.stringify(data),
		contentType: "application/json; charset=utf-8", dataType: "json",
		success: function(data){successFunction(data);},
		failure: function(errMsg) {console.log(errMsg);}
	});
};

// HTML functions
addPortButton = function(item, value) {
	$("#btn-port-group").append("<label class=\"btn btn-secondary\" id=\"btn-port-" + item + "\"><input type=\"radio\" name=\"options\" autocomplete=\"off\">" + value + "</label>");
};

addModeLink = function(item, value) {
	$("#drop-mode").append("<a class=\"dropdown-item\" id=\"mode-" + item + "\" href=\"#\">" + value + "</a>");
	$("#mode-" + item).click(function() {
		$("#form-mode").val(value.toString());
	});
};

// Visible data
update = function() {
	send({"CMD":["GETLIGHT", "GETSERIAL", "STATUS", "PORTS", "LISTOPTIONS"]}, function(data) {
		$("#panel-server-response").html(JSON.stringify(data));

		// status
		$("#form-port").val(data.STATUS.PORT);
		$("#btn-port-group").empty();
		if (data.PORTS.length > 0) {
			$("#btn-noports").hide();
			$("#btn-connect").prop('disabled', false);
			$.each(data.PORTS, function(index, value) {
				addPortButton(index, value);
			});

			//select first or active port
			if (data.STATUS.PORT == "DISCONNECTED") {
				$("#btn-port-0").addClass('active');
				$("#btn-disconnect").hide();
				$("#btn-connect").show();
				$("#btn-port-group").show();
			}
			else {
				$("#btn-port-" + $.inArray(data.STATUS.PORT, data.PORTS)).addClass('active');
				$("#btn-connect").hide();
				$("#btn-port-group").hide();
				$("#btn-disconnect").show();
			}
		} else {
			$("#btn-port-group").hide();
			$("#btn-disconnect").hide();
			$("#btn-connect").prop('disabled', true);
			$("#btn-connect").show();
			$("#btn-noports").show();
		}		

		// light
		$("#form-red").val(data.GETLIGHT.R);
		$("#form-green").val(data.GETLIGHT.G);
		$("#form-blue").val(data.GETLIGHT.B);
		$("#form-alpha").val(data.GETLIGHT.A);
		$("#form-mode").val(data.GETLIGHT.MODE);
		$("#form-interval").val(data.GETLIGHT.INTERVAL);

		// light modes
		$("#drop-mode").empty();
		if (data.LISTOPTIONS.MODE.length > 0) {
			$.each(data.LISTOPTIONS.MODE, function(index, value) {
				addModeLink(index, value);
			});
		}

		// serial
		$("#form-delay").val(data.GETSERIAL.DELAY);
		$("#form-baudrate").val(data.GETSERIAL.BAUDRATE);
		$("#form-databits").val(data.GETSERIAL.DATABITS);
		$("#form-stopbits").val(data.GETSERIAL.STOPBITS);
		$("#form-parity").val(data.GETSERIAL.PARITY);
		$("#check-onlysendonchange").prop("checked", data.GETSERIAL.ONLYSENDONCHANGE);
	});
};
update();

save = function() {
	send({
		"CMD": ["SETLIGHT", "SETSERIAL"],
		"R":$("#form-red").val(),
		"G":$("#form-green").val(),
		"B":$("#form-blue").val(),
		"A":$("#form-alpha").val(),
		"MODE":$("#form-mode").val(),
		"INTERVAL":$("#form-interval").val(),
		"DELAY":$("#form-delay").val(),
		"BAUDRATE":$("#form-baudrate").val(),
		"DATABITS":$("#form-databits").val(),
		"STOPBITS":$("#form-stopbits").val(),
		"PARITY":$("#form-parity").val(),
		"ONLYSENDONCHANGE":$("#check-onlysendonchange").is(":checked")
	}, function(data) {
		$("#panel-server-response").html(JSON.stringify(data));
	});
};

connect = function() {
	var port = "";
    $("#btn-port-group .active").each(function(){
        port = $(this).text();
    });

	send({"CMD":"CONNECT", "PORT": port}, function(data) {
		update();
	});
};

disconnect = function() {
	send({"CMD":"DISCONNECT"}, function(data) {
		update();
	});
};

// Button functionality
$("#btn-update").click(update);
$("#btn-noports").click(update);
$("#btn-connect").click(connect);
$("#btn-disconnect").click(disconnect);
$("#btn-save").click(save);
