#!/bin/bash
# Petrusan George-Cristian 5.1

logger()
{
	if [[ -e /var/log/$1 ]]
	then
		if [[ $1 == *.gz ]]
		then
			gunzip -c /var/log/$1 | grep $2
		else
			grep $2 /var/log/$1
		fi
	fi
}

while true
do
	clear
	echo '--------MENIU--------'
	echo '1. Ieșire'
	echo '2. Afișare: numele serverului, tipul sistemului de operare, versiunea bash-ului și calea curentă (vezi $HOSTNAME, $OSTYPE si alte variabile de sistem)'
	echo '3. Afișarea zilei săptămânii în care va fi Ziua Muncii în acest an (vezi date)'
	echo '4. Afișarea primelor 2 shell-urilor instalate în sistem (vezi /etc/shells)'
	echo '5. Afișarea pentru toți utilizatorii din sistem a numelui de login, a user ID-ului si a directorului de home (vezi /etc/passwd)'
	echo '6. Afişare ultimele linii începând cu linia 9 din fișierul /etc/protocols folosind comanda tail.'
	echo '7. Afișare tip procesor şi mărimea cache-ului (vezi /proc/cpuinfo)'
	echo '8. Scrieți comanda ls care afișează toate numele de directoare din directorul /proc care nu reprezintă imaginea unui proces pe disc (numele directorului care reprezintă imaginea unui proces pe disc este format doar din cifre)'
	echo '9. Sa se afiseze toate fisierele de log. Sa se creeze o funcție care primește ca si parametrii cititi de la tastatura un nume de fișier de log (ori curent ori arhivat) și un text, si care va căuta si afișa liniile care conțin textul primit ca si parametru.'
	echo 'introduceti optiunea dumneavoastra:'

	read opt

	clear

	case $opt in
		1 ) exit 1
			;;
		2 )	echo hostname: $HOSTNAME
			echo ostype: $OSTYPE
			echo bash_version: $BASH_VERSION
			echo current path: $PWD
			;;
		3 ) an=$(date "+%Y")
			data=0501
			date --date=$an$data | awk '{ print $1 }'
			;;
		4 ) cat /etc/shells | head -3 | tail -2
			;;
		5 ) cat /etc/passwd | awk -F : '{ print $1 " " $3 " " $6 }'
			;;
		6 ) cat /etc/protocols | tail -n +9
			;;
		7 ) cat /proc/cpuinfo | grep "model name\|cache size" | head -2
			;;
		8 ) ls /proc | grep -v [0-9]
			;;
		9 )	ls /var/log
			echo introduceti numele fisierului/arhivei
			read fisiere
			echo introduceti numele textului cautat
			read text
			logger $fisiere $text
			;;
		* ) echo optiune inexistenta
			;;
	esac
	echo -e '\n\n\npress any key to continue...'

	read w

done
