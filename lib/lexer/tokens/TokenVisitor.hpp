#ifndef TOKENVISITOR_HPP_
#define TOKENVISITOR_HPP_

class EofToken;
class CommentToken;
class NewlineToken;
class PunctToken;
class OperatorToken;
class LiteralToken;
class KeywordToken;
class IdentToken;

struct TokenVisitor {
  virtual ~TokenVisitor() = default;
  virtual void visit(const IdentToken& t) = 0;
  virtual void visit(const KeywordToken& t) = 0;
  virtual void visit(const LiteralToken& t) = 0;
  virtual void visit(const OperatorToken& t) = 0;
  virtual void visit(const PunctToken& t) = 0;
  virtual void visit(const NewlineToken& t) = 0;
  virtual void visit(const CommentToken& t) = 0;
  virtual void visit(const EofToken& t) = 0;
};

#endif // TOKENVISITOR_HPP_
