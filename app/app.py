from flask import Flask, render_template
from flask_restful import Resource, Api, request
import os

app = Flask(__name__)  

def getDetails(form):

	details = []

	if form['singleFileName'] != "":
		details.append("File(single): "+form['singleFileName'])

	if form['forwardFileName'] != "":
		details.append("Forward File: "+form['forwardFileName'])

	if form['reverseFileName'] != "":
		details.append("Reverse File: "+form['reverseFileName'])

	if form['forwardAdapterName'] != "":
		details.append("Adapter(forward): "+form['forwardAdapterName'])

	if form['reverseAdapterName'] != "":
		details.append("Adapter(reverse): "+form['reverseAdapterName'])

	if form['singleAdapterName'] != "":
		details.append("Adapter: "+form['singleAdapterName'])

	if form['singleMismatchName'] != "":
		details.append("Mismatch (5'/3'): "+form['singleMismatchName']+" bases")

	if form['rightMismatchName'] != "":
		temp = float(form['rightMismatchName']) * 100;
		details.append("Incompatibilities in region 3 ': "+str(temp)+"%")

	if form['pairedMismatchName'] != "":
		details.append("Mismatch (5'/3'): "+form['pairedMismatchName']+" bases")

	if form['rightPairedMismatchName'] != "":
		temp = float(form['rightPairedMismatchName']) * 100;
		details.append("Incompatibilities in region 3': "+str(temp)+"%")

	if form['trimQualityName'] != "":
		if form['trimQualityName'] == "n":
			details.append("Trim per quality: Trim (N)")
		elif form['trimQualityName'] == "quality":
			if form['minQualityName'] != "":
				details.append("Trim (Ns) and bases with score <= "+form['minQualityName'])

	return details

@app.route('/')
def home():
	data = "intro"
	return render_template('home.html', data=data)

@app.route('/search', methods=['post'])
def search():
	
	form = ""
	form = request.form.to_dict()
	if not form:
		form = json.loads(request.data.decode())
	
	out = ""
	if(form['layoutName'] == "single"):
		print("================================== Info Log ================================================ ")
		# print("Arquivo Single")
		if(form['singleFileName'] != ""):
			dataSingle = "data/"+form['singleFileName']
			dataSingle = dataSingle.replace(" ", "\ ")
			if(form['identifyRemoveName'] == "onlyIdentify"):
				command = "./FAIR -s "+dataSingle+" --only-identify -o output/"
				out = os.system(command)
				print("====================================== end log ============================================ ")
				details = getDetails(form)
				return render_template('home.html', data="success", details=details)
			else:
				if(form['singleAdapterName'] != ""):
					command = "./FAIR -s "+dataSingle+" --adapter "+form['singleAdapterName']+" --only-remove -o output"

					if(form['singleMismatchName'] != ""):
						command += " -mm "+str(form['singleMismatchName'])
					else:
						form['singleMismatchName'] = "2"

					if(form['rightMismatchName'] != ""):
						command += " -mmr "+str(form['rightMismatchName'])
					else:
						form['rightMismatchName'] = "0.5"

					if(form['trimQualityName'] == "n"):
						command += " --trim";
					elif(form['trimQualityName'] == 'quality'):
						if(form['minQualityName'] != ""):
							command += " --trim-quality "+str(form['minQualityName'])
						else:
							out = "Insert minimum value of quality!"
							print("====================================== end log ============================================ ")
							return render_template('home.html', data=out, form=form)

					
					print("Query: "+command)
					out = os.system(command)
					if(out == 0):
						print("====================================== end log ============================================")
						details = getDetails(form)
						return render_template('home.html', data="success", details=details)

				else:
					out = "Insert a Single Adapter!"
					print("====================================== end log ============================================ ")
					return render_template('home.html', data=out, form=form)

		else:
			out = "Insert Single File!"
			print("====================================== end log ============================================ ")
			return render_template('home.html', data=out, form=form)


	if(form['layoutName'] == "paired"):
		print("================================== Info Log ================================================ ")
		if(form['forwardFileName'] != ""):
			dataForward = "data/"+form['forwardFileName']
			dataForward = dataForward.replace(" ", "\ ")

			if(form['reverseFileName'] != ""):
				dataReverse = "data/"+form['reverseFileName']
				dataReverse = dataReverse.replace(" ", "\ ")

				if(form['identifyRemoveName'] == "onlyIdentify"):
					out = os.system("./FAIR -f "+dataForward+" -r "+dataReverse+" --only-identify -o output/")
					print("====================================== end log ============================================ ")
					details = getDetails(form)
					return render_template('home.html', data="success", details=details)
				else:
					if(form['forwardAdapterName'] != ""):
						forwardAdapter = form['forwardAdapterName']
						command = "./FAIR --only-remove -o output -f "+dataForward+" -r "+dataReverse+" --forward-adapter "+forwardAdapter
					else:
						out = "Insira um Adaptador Forward"
						print("====================================== end log ============================================ ")
						return render_template('home.html', data=out, form=form)

					if(form['reverseAdapterName'] != ""):
						reverseAdapter = form['reverseAdapterName']
						command += " --reverse-adapter "+reverseAdapter
					else:
						out = "Insert Adapter Reverse"
						print("====================================== end log ============================================ ")
						return render_template('home.html', data=out, form=form)

					if(form['pairedMismatchName'] != ""):
						command += " -mm "+str(form['pairedMismatchName'])
					else:
						form['pairedMismatchName'] = "2"

					if(form['rightPairedMismatchName'] != ""):
						command += " -mmr "+str(form['rightPairedMismatchName'])
					else:
						form['rightPairedMismatchName'] = "0.5"

					if(form['trimQualityName'] == "n"):
						command += " --trim";
					elif(form['trimQualityName'] == 'quality'):
						if(form['minQualityName'] != ""):
							command += " --trim-quality "+str(form['minQualityName'])
						else:
							out = "Insert the minimum value of quality!"
							print("====================================== end log ============================================ ")
							return render_template('home.html', data=out, form=form)

					# print(command)
					out = os.system(command)
					if(out == 0):
						print("====================================== end log ============================================ ")
						details = getDetails(form)
						return render_template('home.html', data="success", details=details)

			else:
				out = "Insert reverse file!"
				print("====================================== end log ============================================ ")
				return render_template('home.html', data=out, form=form)

		else:
			out = "Insert forward file"
			print("====================================== end log ============================================ ")
			return render_template('home.html', data=out, form=form)

		# return '0' case sucess
	print("====================================== end log ============================================ ")
	details = getDetails(form)
	return render_template('home.html', data="success", details=details)

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=8080, debug=True)