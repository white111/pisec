import subprocess 
import Image 
import datetime
import StringIO 

akttime = str(datetime.datetime.now())
dir = "/var/www/imgs/"
proc = subprocess.Popen(['/opt/vc/bin/raspistill', '-e', 'jpg', '-w', "1024", "-h", "768",  "-q", "70", "-o", "-"] , stdout = subprocess.PIPE)
imgdata, err = proc.communicate()
image = Image.open(StringIO.StringIO(imgdata))
image.save(dir + akttime + "-" + "violation" + ".jpg")
msg = akttime.replace(" ","-") + "-" + "violation"

//add your url for SMS API here, with data required by your operator
url =  "xxxxxxxxx"
sendsms = subprocess.Popen(['curl', '-sS', url ])


