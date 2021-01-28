type t('a, 'p, 'e) =
  | NotAsked
  | Loading('p)
  | Failure('e)
  | Success('a);

let (@!) = (g, f, x) => g(f(x));

let andMap = (wrappedValue, wrappedFunction) =>
  switch (wrappedFunction, wrappedValue) {
  | (Success(f), Success(value)) => Success(f @@ value)
  | (Failure(error), _) => Failure(error)
  | (_, Failure(error)) => Failure(error)
  | (Loading(p), _) => Loading(p)
  | (_, Loading(p)) => Loading(p)
  | (NotAsked, _) => NotAsked
  | (_, NotAsked) => NotAsked
  };

let map = (f, data) =>
  switch (data) {
  | Success(value) => Success(f @@ value)
  | Failure(e) => Failure(e)
  | Loading(p) => Loading(p)
  | NotAsked => NotAsked
  };

let map2 = (f, a, b) => map(f, a) |> andMap(b);

let map3 = (f, a, b, c) => map(f, a) |> andMap(b) |> andMap(c);

let mapError = (f, data) =>
  switch (data) {
  | Success(x) => Success(x)
  | Failure(e) => Failure(f @@ e)
  | Loading(y) => Loading(y)
  | NotAsked => NotAsked
  };

let mapBoth = (successFn, errorFn) => mapError(errorFn) @! map(successFn);

let andThen = (f, data) =>
  switch (data) {
  | Success(a) => f(a)
  | Failure(v) => Failure(v)
  | NotAsked => NotAsked
  | Loading(v) => Loading(v)
  };

let withDefault = (default, data) =>
  switch (data) {
  | Success(x) => x
  | _ => default
  };

let fromResult =
  Belt.Result.(
    fun
    | Ok(x) => Success(x)
    | Error(e) => Failure(e)
  );

let toOption =
  fun
  | Success(a) => Some(a)
  | _ => None;

let append = (a, b) => map((a, b) => (a, b), a) |> andMap(b);

let succeed = a => Success(a);

let isSuccess =
  fun
  | Success(_a) => true
  | _ => false;

let isFailure =
  fun
  | Failure(_e) => true
  | _ => false;

let isLoading =
  fun
  | Loading(_) => true
  | _ => false;

let isNotAsked =
  fun
  | NotAsked => true
  | _ => false;