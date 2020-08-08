from flask import Flask, render_template
from flask_restful import Resource, Api, request
import os

app = Flask(__name__)  
 
@app.route('/')
def home():
	data = "intro"
	return render_template('home.html', data=data)

@app.route('/search', methods=['post'])
def search():
	
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
				out = os.system("./FAIR -s "+dataSingle+" --only-identify -o output/")
				print("====================================== end log ============================================ ")
				return render_template('home.html', data="success")
			else:
				if(form['singleAdapterName'] != ""):
					command = "./FAIR -s "+dataSingle+" --adapter "+form['singleAdapterName']+" --only-remove -o output"

					if(form['singleMismatchName'] != ""):
						command += " -mm "+str(form['singleMismatchName'])

					if(form['trimQualityName'] == "n"):
						command += " --trim";
					elif(form['trimQualityName'] == 'quality'):
						if(form['minQualityName'] != ""):
							command += " --trim-quality --min-quality "+str(form['minQualityName'])
						else:
							out = "Insira o valor mínimo de qualidade!"
							print("====================================== end log ============================================ ")
							return render_template('home.html', data=out)

					
					print("Query: "+command)
					out = os.system(command)
					if(out == 0):
						print("====================================== end log ============================================")
						return render_template('home.html', data="success")

				else:
					out = "Insira um Adaptador Single"
					print("====================================== end log ============================================ ")
					return render_template('home.html', data=out, onlyRemove=True)

		else:
			out = "Anexar arquivo Single"
			print("====================================== end log ============================================ ")
			# os.system("quit")
			return render_template('home.html', data=out)


	if(form['layoutName'] == "paired"):
		print("================================== Info Log ================================================ ")
		# print("Arquivo Paired")
		if(form['forwardFileName'] != ""):
			dataForward = "data/"+form['forwardFileName']
			dataForward = dataForward.replace(" ", "\ ")

			if(form['reverseFileName'] != ""):
				dataReverse = "data/"+form['reverseFileName']
				dataReverse = dataReverse.replace(" ", "\ ")

				if(form['identifyRemoveName'] == "onlyIdentify"):
					out = os.system("./FAIR -f "+dataForward+" -r "+dataReverse+" --only-identify -o output/")
					print("====================================== end log ============================================ ")
					return render_template('home.html', data="success")
				else:
					if(form['forwardAdapterName'] != ""):
						forwardAdapter = form['forwardAdapterName']
						command = "./FAIR --only-remove -o output -f "+dataForward+" -r "+dataReverse+" --forward-adapter "+forwardAdapter
					else:
						out = "Insira um Adaptador Forward"
						print("====================================== end log ============================================ ")
						return render_template('home.html', data=out)

					if(form['reverseAdapterName'] != ""):
						reverseAdapter = form['reverseAdapterName']
						command += " --reverse-adapter "+reverseAdapter
					else:
						out = "Insira um Adaptador Reverse"
						print("====================================== end log ============================================ ")
						return render_template('home.html', data=out)

					if(form['pairedMismatchName'] != ""):
						command += " -mm "+str(form['pairedMismatchName'])

					if(form['trimQualityName'] == "n"):
						command += " --trim";
					elif(form['trimQualityName'] == 'quality'):
						if(form['minQualityName'] != ""):
							command += " --trim-quality --min-quality "+str(form['minQualityName'])
						else:
							out = "Insira o valor mínimo de qualidade!"
							print("====================================== end log ============================================ ")
							return render_template('home.html', data=out)

					# print(command)
					out = os.system(command)
					if(out == 0):
						print("====================================== end log ============================================ ")
						return render_template('home.html', data="success")

			else:
				out = "Anexar arquivo reverse"
				print("====================================== end log ============================================ ")
				return render_template('home.html', data=out)

		else:
			out = "Anexar arquivo forward"
			print("====================================== end log ============================================ ")
			return render_template('home.html', data=out)

		# retorna '0' caso tenha sucesso
	print("====================================== end log ============================================ ")
	return render_template('home.html', data="success")

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=8080, debug=True)