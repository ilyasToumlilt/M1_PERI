import os, time, re

# Programme serveur :
# Ce programme va recevoir sur le canal f2s des chaines de la 
# forme <id>:<str>
# et devra renvoyer sur le canal s2f une chaine de la 
# forme <id>:<len str>

# initialisation des tubes de communication :
f2s = '/tmp/monsuperf2s';
s2f = '/tmp/monsupers2f';

if not os.path.exists(f2s):
    os.mkfifo(f2s)

if not os.path.exists(s2f):
    os.mkfifo(s2f)

pipe_in  = open(f2s, 'r')
pipe_out = open(s2f, 'w')

#main loop :
while 1:
    # lecture bloquante sur le canal
    buf = pipe_in.readline();
    
    # parse de l'id et la str par regex
    m = re.search(r'([0-9]+)\:(.*)', buf);
    if m is not None :
        idn = m.group(1);
        cmd = m.group(2);

    # latence serveur
    time.sleep(5)

    # construction et envoi de la chaine
    buf = idn + ":" + str(len(cmd))
    pipe_out.write(buf)
    pipe_out.flush()
