#let journal_entry(date: datetime, doc) = {
  pagebreak()
  set align(center)
  set text(size: 20pt)
  heading(date.display("[day] [month repr:long] [year]"))
  set align(left)
  set text(size: 14pt, font: "Georgia")
  doc
}