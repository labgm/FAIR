// $(document).ready(function() {
	
// VERIFICACAO INICIAL DE PARAMETROS SELECIONADOS
  	var value = document.getElementById("cbLayoutId").value;
 	if(value === "single")
 	{
		 value = document.getElementById("cbIdentifyRemoveId").value;
		 disable_items_paired();
		 remove_disable_items_single();
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
		disable_items_single();
		remove_disable_items_paired();
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
		clean_inputs_quality_trimming();
		clean_inputs_param_adapters();
		disable_inputs_param_adapters();
		disable_inputs_quality_trimming();
		
		document.getElementById("panelQualityTrimId").style.display = "none";
  
		// document.getElementById("panelPhredId").style.display = 'none';
		document.getElementById("panelSingleAdaptersId").style.display = 'none';
		document.getElementById("panelPairedAdaptersId").style.display = 'none';
 	}
 	else if(value === "onlyRemove")
 	{
		remove_disable_inputs_param_adapters();
		remove_disable_inputs_quality_trimming();

  		document.getElementById("btnTrimIdentifyId").innerHTML = "Trim";

		 var layout = document.getElementById("cbLayoutId").value;
		 document.getElementById("panelQualityTrimId").style.display = "block";

 		if(layout === "single")
 		{
 			document.getElementById("panelSingleAdaptersId").style.display = 'block';
			document.getElementById("panelPairedAdaptersId").style.display = 'none';

			remove_disable_items_single();

 		}
 		else if(layout === "paired")
 		{
 			document.getElementById("panelPairedAdaptersId").style.display = 'block';
			document.getElementById("panelSingleAdaptersId").style.display = 'none';

			remove_disable_items_paired();
		}
 	}

 	// ===================================================================


  $("#cbLayoutId").on('change', function(e){

  	var value = document.getElementById("cbLayoutId").value;
 	if(value === "single")
 	{
		 value = document.getElementById("cbIdentifyRemoveId").value;
		 disable_items_paired();
		 remove_disable_items_single();
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
		 disable_items_single();
		 remove_disable_items_paired();
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
	  clean_inputs_quality_trimming();
	  clean_inputs_param_adapters();
	  disable_inputs_param_adapters();
	  disable_inputs_quality_trimming();
	  
	  document.getElementById("panelQualityTrimId").style.display = "none";

 		// document.getElementById("panelPhredId").style.display = 'none';
 		document.getElementById("panelSingleAdaptersId").style.display = 'none';
		 document.getElementById("panelPairedAdaptersId").style.display = 'none';
		 
 	}
 	else if(value === "onlyRemove")
 	{

		remove_disable_inputs_param_adapters();
		remove_disable_inputs_quality_trimming();

  		document.getElementById("btnTrimIdentifyId").innerHTML = "Trim";

		 var layout = document.getElementById("cbLayoutId").value;
		 document.getElementById("panelQualityTrimId").style.display = "block";

 		if(layout === "single")
 		{
 			document.getElementById("panelSingleAdaptersId").style.display = 'block';
			document.getElementById("panelPairedAdaptersId").style.display = 'none';

			remove_disable_items_single();

 		}
 		else if(layout === "paired")
 		{
 			document.getElementById("panelPairedAdaptersId").style.display = 'block';
			document.getElementById("panelSingleAdaptersId").style.display = 'none';

			remove_disable_items_paired();
		}
 	}


	});



function clean_inputs_param_adapters()
{
	var objetos_id = ['singleAdapterId', 'singleMismatchNameId', 'rightMismatchNameId', 'forwardAdapterNameId', 'reverseAdapterNameId', 'pairedMismatchNameId', 'rightPairedMismatchNameId'];
	for(var i = 0; i < objetos_id.length; i++){
		document.getElementById(objetos_id[i]).value = "";
	}
}
function clean_inputs_quality_trimming()
{
	document.getElementById("panelQualityId").style.display = "none";
	document.getElementById("activeQualityTrimId").checked = false;
	var objetos_id = ['minQualityId', 'qualityWindowSizeId', 'maxNId', 'minReadLengthId'];
	for(var i = 0; i < objetos_id.length; i++)
	{
		document.getElementById(objetos_id[i]).value = "";
	}
	document.getElementById("trimNFlankId").checked = false;
}
function disable_inputs_param_adapters()
{
	var objetos_id = ['singleAdapterId', 'singleMismatchNameId', 'rightMismatchNameId', 'forwardAdapterNameId', 'reverseAdapterNameId', 'pairedMismatchNameId', 'rightPairedMismatchNameId'];
	for(var i = 0; i < objetos_id.length; i++){
		$("#"+objetos_id[i]).attr("disabled", "disabled");
	}
}
function disable_inputs_quality_trimming()
{
	document.getElementById("panelQualityId").style.display = "none";
	// document.getElementById("activeQualityTrimId").checked = false;
	var objetos_id = ['minQualityId', 'qualityWindowSizeId', 'maxNId', 'minReadLengthId'];
	for(var i = 0; i < objetos_id.length; i++)
	{
		$("#"+objetos_id[i]).attr("disabled", "disabled");
	}
	document.getElementById("trimNFlankId").checked = false;
}
function remove_disable_inputs_param_adapters()
{
	var value = document.getElementById("cbLayoutId").value;
	var objetos_id = [];
	if(value === "single"){
		objetos_id = ['singleAdapterId', 'singleMismatchNameId', 'rightMismatchNameId'];
	}else if(value === "paired"){
		objetos_id = ['forwardAdapterNameId', 'reverseAdapterNameId', 'pairedMismatchNameId', 'rightPairedMismatchNameId'];
	}

	for(var i = 0; i < objetos_id.length; i++){
		$("#"+objetos_id[i]).removeAttr("disabled");
	}
}
function remove_disable_inputs_quality_trimming()
{
	// document.getElementById("panelQualityId").style.display = "none";
	// document.getElementById("activeQualityTrimId").checked = false;
	var objetos_id = ['minQualityId', 'qualityWindowSizeId', 'maxNId', 'minReadLengthId'];
	for(var i = 0; i < objetos_id.length; i++)
	{
		$("#"+objetos_id[i]).removeAttr("disabled");
	}
	document.getElementById("trimNFlankId").checked = false;
}



function disable_items_single()
{
	var objetos_id = ['singleFileId','singleAdapterId', 'singleMismatchNameId', 'rightMismatchNameId'];
	for(var i = 0; i < objetos_id.length; i++){
		$("#"+objetos_id[i]).attr("disabled", "disabled");
	}
}
function disable_items_paired()
{
	var objetos_id = ['forwardFileId','reverseFileId', 'forwardAdapterNameId', 'reverseAdapterNameId', 'pairedMismatchNameId', 'rightPairedMismatchNameId'];
	for(var i = 0; i < objetos_id.length; i++){
		$("#"+objetos_id[i]).attr("disabled", "disabled");
	}
}
function remove_disable_items_single()
{
	var objetos_id = ['singleFileId','singleAdapterId', 'singleMismatchNameId', 'rightMismatchNameId'];
	for(var i = 0; i < objetos_id.length; i++){
		$("#"+objetos_id[i]).removeAttr("disabled");
	}
}
function remove_disable_items_paired()
{
	var objetos_id = ['forwardFileId','reverseFileId', 'forwardAdapterNameId', 'reverseAdapterNameId', 'pairedMismatchNameId', 'rightPairedMismatchNameId'];
	for(var i = 0; i < objetos_id.length; i++){
		$("#"+objetos_id[i]).removeAttr("disabled");
	}
}

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
	// Confere se 'Quality Trim' está marcado
	if(!document.getElementById("activeQualityTrimId").checked){
		document.getElementById("panelQualityId").style.display = 'none';
		disable_inputs_quality_trimming();
	}else{
		document.getElementById("panelQualityId").style.display = 'block';
	}

	// CHECK BOX ACTIVE TRIMMING PER QUALITY
	$('#activeQualityTrimId').change(function() {
		if(!this.checked) {		
			clean_inputs_quality_trimming();
			disable_inputs_quality_trimming();
		}else{
			remove_disable_inputs_quality_trimming();
			document.getElementById("panelQualityId").style.display = "block";
		}
	});

// });