#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

ADNETCOIND=${ADNETCOIND:-$SRCDIR/adnetcoind}
ADNETCOINCLI=${ADNETCOINCLI:-$SRCDIR/adnetcoin-cli}
ADNETCOINTX=${ADNETCOINTX:-$SRCDIR/adnetcoin-tx}
ADNETCOINQT=${ADNETCOINQT:-$SRCDIR/qt/adnetcoin-qt}

[ ! -x $ADNETCOIND ] && echo "$ADNETCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
ADNETVER=($($ADNETCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for adnetcoind if --version-string is not set,
# but has different outcomes for adnetcoin-qt and adnetcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$ADNETCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $ADNETCOIND $ADNETCOINCLI $ADNETCOINTX $ADNETCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${ADNETVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${ADNETVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
