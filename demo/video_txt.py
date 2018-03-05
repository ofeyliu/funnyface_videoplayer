# -*- coding: utf-8 -*-
"""
Created on Wed Nov 16 19:44:45 2016

@author: lww
"""

import requests
import base64
import cv2

video_name='chu2.mp4'
cap = cv2.VideoCapture(video_name)
#size = (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)),int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
#fourcc = cv2.VideoWriter_fourcc(*'XVID')
#videoWriter = cv2.VideoWriter('testResult3.mp4',fourcc,25.0,(640,480))
file_name=video_name.split('.')[0]
frame_file=open('frame_'+file_name+'.txt','w')
keypt_file=open('keypt_'+file_name+'.txt','w')
attri_file=open('attri_'+file_name+'.txt','w')
quality_file=open('quality_'+file_name+'.txt','w')

while(cap.isOpened()):
    status, img = cap.read()
    if status==True:
        img = cv2.resize(img,(640,480),interpolation=cv2.INTER_CUBIC)
        imgEncode = cv2.imencode('.png',img)[1]
        imgBase64 = base64.b64encode(imgEncode)
#云从API
        param = {
        'app_id': 'liuweiwei',
        'app_secret': '6180542639080541657',
        'img': imgBase64
         }
        r = requests.post('http://120.25.161.56:7000/face/tool/keypt', param)
        if (r.json()['result'])==0:
            faces=r.json()['faces']
            for face_id,face in enumerate(faces):
                frame_file.write(str(face_id)+' '+str(face['x'])+' '+str(face['y'])+' '+str(face['width'])+' '+str(face['height'])+'\n')
                for point_id,point in enumerate(face['keypt']):
                    keypt_file.write(str(face_id)+' '+str(point_id)+' '+str(point['x'])+' '+str(point['y'])+'\n')

        r2 = requests.post('http://120.25.161.56:7000/face/tool/attribute', param)
        if (r2.json()['result'])==0:
            faces=r2.json()['faces']
            for face_id,face in enumerate(faces):
                attri_file.write(str(face_id)+' '+str(face['age'])+' '+str(face['gender'])+' '+str(face['x'])+' '+str(face['y'])+' '+str(face['width'])+' '+str(face['height'])+'\n')
                
        r3 = requests.post('http://120.25.161.56:7000/face/tool/quality', param)
        if (r3.json()['result'])==0:
            quality_file.write(str(r3.json()['score'])+' '+str(r3.json()['info'])+'\n')
            
        if cv2.waitKey(60) & 0xFF == ord('q'):
            break
    else:
        break    
cap.release()
frame_file.close()
keypt_file.close()
attri_file.close()
quality_file.close()

