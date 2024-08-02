#!/bin/bash
set -e
set -v

PRIVATE_KEY=ec_private_key.pem
PUBLIC_KEY=ec_public_key.pem

[ -e $PRIVATE_KEY ] || [ -e $PUBLIC_KEY ] && { echo "Keys already exist. Not overwriting."; exit 1; }


openssl ecparam -genkey -name secp256k1 -noout -out $PRIVATE_KEY
openssl ec -in $PRIVATE_KEY -pubout -out $PUBLIC_KEY
