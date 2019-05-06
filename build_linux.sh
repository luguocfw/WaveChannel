#! /bin/bash
ROOT=`pwd`
GYP_TOOLS=${ROOT}/tools/GYP-Tools/gyp


if [ ! -d ${ROOT}/tools ]
then
  mkdir ${ROOT}/tools
fi

if [ ! -d ${ROOT}/tools/GYP-Tools ]
then
  cd ${ROOT}/tools
  git clone https://github.com/luguocfw/GYP-Tools
  cd ${ROOT}
fi
export GYP_DEFINES="OS=linux PRO_ROOT=${ROOT} "
${GYP_TOOLS} --depth=. -f make wave_trans.gyp
