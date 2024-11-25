#let format_question(question: [], answer: [], hebrew: false) = {
  set align(left) if not hebrew
  set align(right) if hebrew
  if question != [] {
    rect(fill: silver, stroke: 1pt + black, width: 100%, radius: 0.2em, [
      #set text(font: "Arial", style: "italic")
      #question
    ])
  }
  parbreak()
  answer
}

#let homework_doc(assignment_number: int, title: "", authors: (), autoHeading: true, hebrew: false, numbering: true, doc) = {
  set align(center)
  set text(12pt)

  set text(font: "Baskerville") if not hebrew
  set text(font: "David", dir: rtl) if hebrew
  set text(lang: "en") if not hebrew
  set text(lang: "he") if hebrew
  set page(paper: "a4")
  set page(numbering: "1") if numbering
  set page(margin: 2cm)

  text(17pt)[
    #if title != "" [#title] else [Assignment #assignment_number]
  ]
  let count = authors.len()
  let ncols = calc.min(count, 3)
  grid(
    columns: (1fr,) * ncols,
    row-gutter: 24pt,
    ..authors.map(author => [
      #author.name \
      #author.id
    ]),
  )

  set par(justify: true)
  
  set heading(numbering: "1.1") if autoHeading

  show heading.where(level: 1): it => {
    set align(center)
    set text(15pt, weight: "bold")
    it
  }

  show "Schrodinger": "Schrödinger"

  set align(left) if not hebrew
  set align(right) if hebrew
  set math.vec(delim: "[")
  set math.mat(delim: "[")
  doc
}

#let enable_english(doc) = {
  set text(lang: "en")
  set text(dir: ltr)
  set align(left)
  doc
}

#let enable_hebrew(doc) = {
  set text(lang: "he")
  set text(dir: rtl)
  set align(right)
  doc
}


#import "@preview/physica:0.9.2": *

#let nket(..elements) = {
  let arg = elements.pos().join(", ");
  $ket(#arg)$
}

#let nbra(..elements) = {
  let arg = elements.pos().join(", ");
  $bra(#arg)$
}

#let dyad(..elements) = {
  nket(..elements) 
  nbra(..elements)
}

#let mspace = h(1cm)
