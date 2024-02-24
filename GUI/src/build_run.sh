#!/bin/zsh

echo "Enter Passphrase: "
read -s passphrase

if gpg --quiet --batch --yes --decrypt --passphrase="$passphrase" --output decrypt_file.sh build_and_run.sh.gpg; then
    source decrypt_file.sh
fi

rm decrypt_file.sh