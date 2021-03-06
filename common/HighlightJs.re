[@bs.deriving abstract]
type highlightResult = {value: string};

[@bs.module "highlight.js/lib/highlight"]
external highlight: (~lang: string, ~value: string) => highlightResult =
  "highlight";

let renderHLJS =
    (
      ~highlightedLines=[||],
      ~darkmode=false,
      ~code: string,
      ~lang: string,
      (),
    ) => {
  // If the language couldn't be parsed, we will fall back to text
  let (lang, highlighted) =
    try((lang, highlight(~lang, ~value=code)->valueGet)) {
    | Js.Exn.Error(_) => ("text", code)
    };

  // Add line highlighting as well
  let highlighted =
    if (Belt.Array.length(highlightedLines) > 0) {
      Js.String2.split(highlighted, "\n")
      ->Belt.Array.mapWithIndex((i, line) =>
          if (Js.Array2.find(highlightedLines, lnum => lnum === i + 1) !== None) {
            "<span class=\"hljs-line-highlight\">" ++ line ++ "</span>";
          } else {
            line;
          }
        )
      ->Js.Array2.joinWith("\n");
    } else {
      highlighted;
    };

  let dark = darkmode ? "dark" : "";

  ReactDOMRe.createElementVariadic(
    "code",
    ~props=
      ReactDOMRe.objToDOMProps({
        "className": "hljs lang-" ++ lang ++ " " ++ dark,
        "dangerouslySetInnerHTML": {
          "__html": highlighted,
        },
      }),
    [||],
  );
};
