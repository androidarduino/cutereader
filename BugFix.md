# Introduction #

Memo for all bugs found

# Details #
  * [r20](https://code.google.com/p/cutereader/source/detail?r=20) bug in ChannelModel::index causes crash
  * RSSDocument::setContent accepts QString, which calls the codec to convert the encoding. This causes chinese content display problem. Should use QByteArray
  * tagName doesn't support tag format like "atom:link","content:encoded", should use "nodeName" instead