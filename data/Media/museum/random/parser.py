#!/usr/bin/python

import re
import sys
skipstart=0
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


tokens=["name","ambient","diffuse","glow","specular","diffuse_map","diffuse_scale","opacity"]

for file_name in sys.argv:


	skipstart+=1

	if skipstart==1:
		continue

	fp = open(file_name)
	contents = fp.read()
	fp.close()
	contents= contents.replace(r"\\","")
	fp2 = open("regex")
	regexfile = fp2.read()
	fp2.close()
	regexfile= regexfile.replace(r"\\","")	
	matchObj = re.match((regexfile), contents, re.M|re.I)

	if matchObj is None:
		print bcolors.FAIL+"error, no match found for "+ bcolors.OKGREEN+ file_name
		continue

	print bcolors.OKBLUE,len(matchObj.groups()),"matches found for "+ bcolors.OKGREEN+file_name
	for group in matchObj.groups():
		print bcolors.ENDC +group

	fp3 = open("template")
	templatefile = fp3.read()
	fp3.close()

	for token in tokens:
		templatefile=templatefile.replace("%{"+token+"}",matchObj.group(token))
	
	if(matchObj.group('opacity')=='alpha_blend'):
		templatefile=templatefile.replace("%(opacitydecl)","opacity-diffuse		1\nopacity-sharp		1\nopacity-cutoff		10\n")
	else:
		templatefile=templatefile.replace("%(opacitydecl)","")


	final = open("DS_"+matchObj.group("name")+".ds.material", 'w')
	final.write(templatefile)
	final.close()
