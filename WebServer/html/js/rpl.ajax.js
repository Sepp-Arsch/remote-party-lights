// Data handling functions
send = function(data, successFunction){
	$.ajax({
		type: "POST", url: "/", data: JSON.stringify(data),
		contentType: "application/json; charset=utf-8", dataType: "json",
		success: function(data){successFunction(data);},
		failure: function(errMsg) {console.log(errMsg);}
	});
};

// Visible data
update = function() {
	send({"CMD":["GETLIGHT", "GETSERIAL", "STATUS", "PORTS", "LISTOPTIONS"]}, function(data) {
		$("#panel-server-response").html(JSON.stringify(data));

		// status
		$("#form-port").val(data.STATUS.PORT);

		// light
		$("#form-red").val(data.GETLIGHT.R);
		$("#form-green").val(data.GETLIGHT.G);
		$("#form-blue").val(data.GETLIGHT.B);
		$("#form-alpha").val(data.GETLIGHT.A);
		$("#form-mode").val(data.GETLIGHT.MODE);
		$("#form-interval").val(data.GETLIGHT.INTERVAL);

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
	send({"CMD":"CONNECT", "PORT": $("#form-port").val()}, function(data) {
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
$("#btn-connect").click(connect);
$("#btn-disconnect").click(disconnect);
$("#btn-save").click(save);
