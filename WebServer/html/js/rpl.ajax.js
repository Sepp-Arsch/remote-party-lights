// Data handling functions

send = function(data, successFunction){
	$.ajax({
		type: "POST", url: "/", data: JSON.stringify(data),
		contentType: "application/json; charset=utf-8", dataType: "json",
		success: function(data){successFunction(data);},
		failure: function(errMsg) {console.log(errMsg);}
	});
};


// Button functionality
$("#btn-status").click(function() {
	send({"CMD":"STATUS"}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
	$("#form-port").val(data.PORT);
});
});

$("#btn-connect").click(function() {
	send({"CMD":"CONNECT", "PORT": $("#form-port").val()}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
});
});

$("#btn-disconnect").click(function() {
	send({"CMD":"DISCONNECT"}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
});
});

$("#btn-ports").click(function() {
	send({"CMD":"PORTS"}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
});
});

$("#btn-get-lights").click(function() {
	send({"CMD":"GETLIGHT"}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
	$("#form-red").val(data.GETLIGHT.R);
	$("#form-green").val(data.GETLIGHT.G);
	$("#form-blue").val(data.GETLIGHT.B);
	$("#form-alpha").val(data.GETLIGHT.A);
	$("#form-mode").val(data.GETLIGHT.MODE);
	$("#form-interval").val(data.GETLIGHT.INTERVAL);
});
});

$("#btn-set-lights").click(function() {
	send({
		"CMD":"SETLIGHT",
		"R":$("#form-red").val(),
		"G":$("#form-green").val(),
		"B":$("#form-blue").val(),
		"A":$("#form-alpha").val(),
		"MODE":$("#form-mode").val(),
		"INTERVAL":$("#form-interval").val()
	}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
});
});

$("#btn-get-serial").click(function() {
	send({"CMD":"GETSERIAL"}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
	$("#form-delay").val(data.GETSERIAL.DELAY);
	$("#form-baudrate").val(data.GETSERIAL.BAUDRATE);
	$("#form-databits").val(data.GETSERIAL.DATABITS);
	$("#form-stopbits").val(data.GETSERIAL.STOPBITS);
	$("#form-parity").val(data.GETSERIAL.PARITY);
	$("#check-onlysendonchange").prop("checked", data.GETSERIAL.ONLYSENDONCHANGE);
});
});

$("#btn-set-serial").click(function() {
	send({
		"CMD":"SETSERIAL",
		"DELAY":$("#form-delay").val(),
		"BAUDRATE":$("#form-baudrate").val(),
		"DATABITS":$("#form-databits").val(),
		"STOPBITS":$("#form-stopbits").val(),
		"PARITY":$("#form-parity").val(),
		"ONLYSENDONCHANGE":$("#check-onlysendonchange").is(":checked")
	}, function(data) {
	$("#panel-server-response").html(JSON.stringify(data));
});
});