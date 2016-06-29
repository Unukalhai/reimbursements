#!/bin/bash
ssh-keyscan -t rsa,dsa,ecdsa -H $travis_deploy_hostname >> $HOME/.ssh/known_hosts
sftp $travis_deploy_username@$travis_deploy_hostname << !
  mkdir $TRAVIS_BUILD_ID
  lcd $TRAVIS_BUILD_DIR/build
  cd $TRAVIS_BUILD_ID
  put -r *
  bye
!
