#let Fasernu = $bold("Faser"nu)$


#let source(url, body) = { 
  link(url)[
  #set text(fill: red)
  #body
]}