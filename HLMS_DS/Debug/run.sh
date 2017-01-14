cp /home/marvin/workspace/HLMS_DS/Debug/libHLMS_DS.so /home/marvin/workspace/HLMS_DS_DEMO/libHLMS_DS.so 
cd /home/marvin/workspace/HLMS_DS_DEMO/
rsync -ra /home/marvin/workspace/HLMS_DS/Media/Hlms/ /home/marvin/workspace/HLMS_DS_DEMO/Media/Hlms/
mkdir /home/marvin/workspace/HLMS_DS_DEMO/OldGLSLCache
mv -v /home/marvin/workspace/HLMS_DS_DEMO/*.glsl /home/marvin/workspace/HLMS_DS_DEMO/OldGLSLCache
echo "done"
