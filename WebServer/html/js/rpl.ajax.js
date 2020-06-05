send = function(data, successFunction){
	$.ajax({
		type: "POST", url: "/", data: JSON.stringify(data),
		contentType: "application/json; charset=utf-8", dataType: "json",
		success: function(data){successFunction(data);},
		failure: function(errMsg) {console.log(errMsg);}
	});
};

test = function(data) {
	console.log("AAAAAAA");
	console.log(data);
}

send({"CMD":"PORTS"}, test);
send({"CMD":"GETLIGHT"}, test);
send({"CMD":"GETSERIAL"}, test);
send({"CMD":"DISCONNECT"}, test);
send({"CMD":"STATUS"}, test);

