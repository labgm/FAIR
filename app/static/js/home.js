// VERIFICACAO INICIAL DE PARAMETROS SELECIONADOS
  var value = document.getElementById("cbTrimQualityId").value;
  if(value === "quality")
 	{
	  	document.getElementById("minQualityId").style.display = 'block';
	  	document.getElementById("minQualityId").focus();
 	}
  	else
  	document.getElementById("minQualityId").style.display = 'none';

  	var value = document.getElementById("cbLayoutId").value;
 	if(value === "single")
 	{
 		value = document.getElementById("cbIdentifyRemoveId").value;
 		if(value === "onlyRemove")
 		{
	 		document.getElementById("panelSingleAdaptersId").style.display = 'block';
	 		document.getElementById("panelPairedAdaptersId").style.display = 'none'; 			
 		}

 		document.getElementById("panelSingleId").style.display = 'block';
 		document.getElementById("panelPairedId").style.display = 'none';
 	}
 	else if(value === "paired")
 	{
 		value = document.getElementById("cbIdentifyRemoveId").value;
 		if(value === "onlyRemove")
 		{
	 		document.getElementById("panelSingleAdaptersId").style.display = 'none';
	 		document.getElementById("panelPairedAdaptersId").style.display = 'block'; 			
 		}
 		document.getElementById("panelPairedId").style.display = 'block';
 		document.getElementById("panelSingleId").style.display = 'none';
 	}

	var value = document.getElementById("cbIdentifyRemoveId").value;
 	if(value === "onlyIdentify")
 	{

  	document.getElementById("btnTrimIdentifyId").innerHTML = "Identify";

		document.getElementById("panelQualityId").style.display = 'none';
 		// document.getElementById("panelPhredId").style.display = 'none';
 		document.getElementById("panelSingleAdaptersId").style.display = 'none';
 		document.getElementById("panelPairedAdaptersId").style.display = 'none';
 	}
 	if(value === "onlyRemove")
 	{

  	document.getElementById("btnTrimIdentifyId").innerHTML = "Trim";

 		var layout = document.getElementById("cbLayoutId").value;

 		document.getElementById("panelQualityId").style.display = 'block';
 		// document.getElementById("panelPhredId").style.display = 'block';

 		if(layout === "single")
 		{
 			document.getElementById("panelSingleAdaptersId").style.display = 'block';
 			document.getElementById("panelPairedAdaptersId").style.display = 'none';
 		}
 		if(layout === "paired")
 		{
 			document.getElementById("panelPairedAdaptersId").style.display = 'block';
 			document.getElementById("panelSingleAdaptersId").style.display = 'none';
		}
 	}

 	// ===================================================================



  $("#cbTrimQualityId").on('change', function(e){

  	var value = document.getElementById("cbTrimQualityId").value;
 	if(value === "quality")
 	{
	  	document.getElementById("minQualityId").style.display = 'block';
	  	document.getElementById("minQualityId").focus();
 	}
  	else
  	document.getElementById("minQualityId").style.display = 'none';

      
  });

  $("#cbLayoutId").on('change', function(e){

  	var value = document.getElementById("cbLayoutId").value;
 	if(value === "single")
 	{
 		value = document.getElementById("cbIdentifyRemoveId").value;
 		if(value === "onlyRemove")
 		{
	 		document.getElementById("panelSingleAdaptersId").style.display = 'block';
	 		document.getElementById("panelPairedAdaptersId").style.display = 'none'; 			
 		}

 		document.getElementById("panelSingleId").style.display = 'block';
 		document.getElementById("panelPairedId").style.display = 'none';
 	}
 	else if(value === "paired")
 	{
 		value = document.getElementById("cbIdentifyRemoveId").value;
 		if(value === "onlyRemove")
 		{
	 		document.getElementById("panelSingleAdaptersId").style.display = 'none';
	 		document.getElementById("panelPairedAdaptersId").style.display = 'block'; 			
 		}
 		document.getElementById("panelPairedId").style.display = 'block';
 		document.getElementById("panelSingleId").style.display = 'none';
 	}


	});

   $("#cbIdentifyRemoveId").on('change', function(e){

  	var value = document.getElementById("cbIdentifyRemoveId").value;
 	if(value === "onlyIdentify")
 	{

  	document.getElementById("btnTrimIdentifyId").innerHTML = "Identify";

		document.getElementById("panelQualityId").style.display = 'none';
 		// document.getElementById("panelPhredId").style.display = 'none';
 		document.getElementById("panelSingleAdaptersId").style.display = 'none';
 		document.getElementById("panelPairedAdaptersId").style.display = 'none';
 	}
 	if(value === "onlyRemove")
 	{

  	document.getElementById("btnTrimIdentifyId").innerHTML = "Trim";

 		var layout = document.getElementById("cbLayoutId").value;

 		document.getElementById("panelQualityId").style.display = 'block';
 		// document.getElementById("panelPhredId").style.display = 'block';

 		if(layout === "single")
 		{
 			document.getElementById("panelSingleAdaptersId").style.display = 'block';
 			document.getElementById("panelPairedAdaptersId").style.display = 'none';
 		}
 		if(layout === "paired")
 		{
 			document.getElementById("panelPairedAdaptersId").style.display = 'block';
 			document.getElementById("panelSingleAdaptersId").style.display = 'none';
		}
 	}


	});

  

function sendForm(){

	var form = document.querySelector(".formId");
	form.method = 'post';
	form.action = '/search';

	var out = form.submit();

	console.log(out);

}

function openModal(modal){

	$('#'+modal).modal("show");

}