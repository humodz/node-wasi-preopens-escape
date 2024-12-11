
cd preopens

while true; do
	rm inside.txt
	echo inside > inside.txt

	ln -sf ../outside.txt inside.txt
done