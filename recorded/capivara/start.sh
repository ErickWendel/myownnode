
if ! command -v "nodemon" /dev/null
then
  npm i -g nodemon --silent
fi

if ! command -v "ccache" /dev/null
then
  sudo apt-get install -y ccache
fi


nodemon -e cc,h,js,cpp,hpp --exec "make && ./bin/capivara index.js"