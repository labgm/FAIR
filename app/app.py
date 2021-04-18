from flask import Flask, render_template
from flask_restful import Resource, Api, request
import os

app = Flask(__name__)  

def getDetails(form):

	details = []

	ir = form["identifyRemoveName"]
	if ir == "onlyRemove":
		details.append(" ONLY REMOVE")
	elif ir == "onlyIdentify":
		details.append(" ONLY IDENTIFY")

	for key, value in form.items():

		if key == "singleFileName":
			details.append(f"File(single): {value}")

		elif key == "forwardFileName":
			details.append(f"Forward File: {value}")

		elif key == "reverseFileName":
			details.append(f"Reverse File: {value}")

		elif key == "forwardAdapterName":
			details.append(f"Adapter(forward): {value}")

		elif key == "reverseAdapterName":
			details.append(f"Adapter(reverse): {value}")

		elif key == "singleMismatchName":
			details.append(f"Max. Mismatch (5'/3'): {value}")

		elif key == "rightMismatchName":
			details.append(f"Incompatibilities in region 3': {float(value)*100}%")

		elif key == "pairedMismatchName":
			details.append(f"Max. Mismatch (5'/3'): {value} nt")

		elif key == "rightPairedMismatchName":
			details.append(f"Incompatibilities in region 3': {float(value)*100}%")

		elif key == "minQualityName":
			details.append(f"Minimum quality: {value}")

		elif key == "qualityWindowSizeName":
			details.append(f"Size quality window: {value}")

		elif key == "maxNName":
			details.append(f"Discard reads containing more than {value} N bases.")

		elif key == "minReadLengthName":
			details.append(f"Minimum size to consider read: {value}")

		elif key == "trimNFlankName":
			details.append("Remove flanking N bases from each read!")

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
		print("================================== info log ================================================ ")
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
					command = f"./FAIR -s {dataSingle} --adapter {form['singleAdapterName']} --only-remove -o output"

					for it in form.items():
						key, value = it

						if key == 'singleMismatchName':
							if(form['singleMismatchName'] != ""):
								command += f" -mm {form['singleMismatchName']}"
							else:
								form['singleMismatchName'] = "2"

						if key == 'rightMismatchName':
							if(form['rightMismatchName'] != ""):
								command += f" -mmr {form['rightMismatchName']}"
							else:
								form['rightMismatchName'] = "0.5"
								command += f" -mmr {form['rightMismatchName']}"

						if key == 'minQualityName':
							if form['minQualityName'] != "":
								command += f" --min-quality {form['minQualityName']}"
							else:
								form['minQualityName'] = "10"
								command += f" --min-quality {form['minQualityName']}"

						if key == 'qualityWindowSizeName':
							if form['qualityWindowSizeName'] != "":
								command += f" --quality-window-size {form['qualityWindowSizeName']}"
							else:
								form['qualityWindowSizeName'] = "4"
								command += f" --quality-window-size {form['qualityWindowSizeName']}"

						if key == 'maxNName':
							if form['maxNName'] != "":
								command += f" --max-n {form['maxNName']}"
							else:
								form['maxNName'] = "10"
								command += f" --max-n {form['maxNName']}"

						if key == 'minReadLengthName':
							if form['minReadLengthName'] != "":
								command += f" --min-read-length {form['minReadLengthName']}"
							else:
								form['minReadLengthName'] = "0"
								command += f" --min-read-length {form['minReadLengthName']}"

						if key == 'trimNFlankName':
							command += " --trim-n-flank "
					
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

					for it in form.items():
						key, value = it
						if key == "forwardAdapterName":
							if(form['forwardAdapterName'] != ""):
								forwardAdapter = form['forwardAdapterName']
								command = "./FAIR --only-remove -o output -f "+dataForward+" -r "+dataReverse+" --forward-adapter "+forwardAdapter
							else:
								out = "Insert a Forward Adapter"
								print("====================================== end log ============================================ ")
								return render_template('home.html', data=out, form=form)

						if key == "forwardAdapterName":
							if(form['reverseAdapterName'] != ""):
								reverseAdapter = form['reverseAdapterName']
								command += " --reverse-adapter "+reverseAdapter
							else:
								out = "Insert a Reverse Adapter"
								print("====================================== end log ============================================ ")
								return render_template('home.html', data=out, form=form)

						elif key == "pairedMismatchName":
							if(form['pairedMismatchName'] != ""):
								command += " -mm "+str(form['pairedMismatchName'])
							else:
								form['pairedMismatchName'] = "2"
								command += " -mm "+str(form['pairedMismatchName'])

						elif key == "rightPairedMismatchName":
							if(form['rightPairedMismatchName'] != ""):
								command += " -mmr "+str(form['rightPairedMismatchName'])
							else:
								form['rightPairedMismatchName'] = "0.5"
								command += " -mmr "+str(form['rightPairedMismatchName'])

						elif key == "minQualityName":
							if(form['minQualityName'] != ""): 
								command += f" --min-quality {form['minQualityName']}"
							else:
								form['minQualityName'] = "5"
								command += f" --min-quality {form['minQualityName']}"

						elif key == "qualityWindowSizeName":
							if(form['qualityWindowSizeName'] != ""):
								command += f" --quality-window-size {form['qualityWindowSizeName']}"
							else:
								form['qualityWindowSizeName'] = "4"
								command += f" --quality-window-size {form['qualityWindowSizeName']}"

						elif key == "maxNName":
							if(form['maxNName'] != ""):
								command += f" --max-n {form['maxNName']}"
							else:
								form['maxNName'] = "0"
								command += f" --max-n {form['maxNName']}"

						elif key == "minReadLengthName":
							if(form['minReadLengthName'] != ""):
								command += f" --min-read-length {form['minReadLengthName']}"
							else:
								form['minReadLengthName'] = "0"
								command += f" --min-read-length {form['minReadLengthName']}"

						elif key == "trimNFlankName":
							command += " --trim-n-flank "

					print(f"Query: {command}")
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